# cartographer_inital_pose
Allows you to set Cartographer's inital pose using RViz's 2D Pose Estimate feature.

## Usage
Start the `initial_pose_listener` node along with Cartographer:
```xml
<node name="cartographer_initial_pose" pkg="cartographer_initial_pose" type="initial_pose_listener" output="screen">
  <param name="configuration_directory" value="path/to/your/configuration_directory" />
  <param name="configuration_basename" value="your_configuration.lua" />
</node>
```

## Node API
Parameters:
- `~relative_to_trajectory_id` (`int32`, default: `0`)
- `~configuration_directory` (`string`)
- `~configuration_basename` (`string`)

Subscribed Topics:
- `initialpose` (`geometry_msgs/PoseWithCovarianceStamped`)

Required Services:
- `get_trajectory_states` (`cartographer_ros_msgs/GetTrajectoryStates`)
- `start_trajectory` (`cartographer_ros_msgs/StartTrajectory`)
- `finish_trajectory` (`cartographer_ros_msgs/FinishTrajectory`)
