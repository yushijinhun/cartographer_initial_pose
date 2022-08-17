#include <cartographer_ros_msgs/FinishTrajectory.h>
#include <cartographer_ros_msgs/GetTrajectoryStates.h>
#include <cartographer_ros_msgs/StartTrajectory.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <ros/ros.h>

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "initial_pose_listener");

  ros::NodeHandle nh;

  int32_t relative_to_trajectory_id = 0;
  ros::param::get("~relative_to_trajectory_id", relative_to_trajectory_id);

  std::string configuration_directory;
  if (!ros::param::get("~configuration_directory", configuration_directory)) {
    ROS_ERROR("~configuration_directory param must be specified.");
    return 1;
  }

  std::string configuration_basename;
  if (!ros::param::get("~configuration_basename", configuration_basename)) {
    ROS_ERROR("~configuration_basename param must be specified.");
    return 1;
  }

  auto get_trajectory_states_srv =
      nh.serviceClient<cartographer_ros_msgs::GetTrajectoryStates>(
          "get_trajectory_states");

  auto start_trajectory_srv =
      nh.serviceClient<cartographer_ros_msgs::StartTrajectory>(
          "start_trajectory");

  auto finish_trajectory_srv =
      nh.serviceClient<cartographer_ros_msgs::FinishTrajectory>(
          "finish_trajectory");

  auto initial_pose_sub =
      nh.subscribe<geometry_msgs::PoseWithCovarianceStamped>(
          "initialpose", 1,
          [&](const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg) {
            // Get all trajectories
            cartographer_ros_msgs::GetTrajectoryStates get_trajectory_states;
            if (!get_trajectory_states_srv.call(get_trajectory_states)) {
              ROS_ERROR("Failed to call get_trajectory_states");
              return;
            }
            if (get_trajectory_states.response.status.code != 0) {
              ROS_ERROR("Failed to call get_trajectory_states: %s",
                        get_trajectory_states.response.status.message.c_str());
              return;
            }

            // Finish the last trajectory
            auto &trajectories =
                get_trajectory_states.response.trajectory_states;
            for (int i = 0; i < trajectories.trajectory_id.size(); i++) {
              auto id = trajectories.trajectory_id[i];
              auto state = trajectories.trajectory_state[i];
              if (state == cartographer_ros_msgs::TrajectoryStates::ACTIVE) {
                ROS_INFO("Finishing active trajectory %d", id);
                cartographer_ros_msgs::FinishTrajectory finish_trajectory;
                finish_trajectory.request.trajectory_id = id;
                if (!finish_trajectory_srv.call(finish_trajectory)) {
                  ROS_ERROR("Failed to call finish_trajectory");
                  return;
                }
                if (finish_trajectory.response.status.code != 0) {
                  ROS_ERROR("Failed to call finish_trajectory: %s",
                            finish_trajectory.response.status.message.c_str());
                  return;
                }
              }
            }

            // Start new trajectory
            cartographer_ros_msgs::StartTrajectory start_trajectory;
            start_trajectory.request.use_initial_pose = true;
            start_trajectory.request.initial_pose = msg->pose.pose;
            start_trajectory.request.relative_to_trajectory_id =
                relative_to_trajectory_id;
            start_trajectory.request.configuration_directory =
                configuration_directory;
            start_trajectory.request.configuration_basename =
                configuration_basename;
            if (!start_trajectory_srv.call(start_trajectory)) {
              ROS_ERROR("Failed to call start_trajectory");
              return;
            }
            if (start_trajectory.response.status.code != 0) {
              ROS_ERROR("Failed to call start_trajectory: %s",
                        start_trajectory.response.status.message.c_str());
              return;
            }

            ROS_INFO("New trajectory started: %d",
                     start_trajectory.response.trajectory_id);
          });

  ros::spin();
}
