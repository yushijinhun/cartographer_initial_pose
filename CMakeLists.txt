cmake_minimum_required(VERSION 3.0.2)
project(cartographer_initial_pose)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    geometry_msgs
    cartographer_ros_msgs
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

catkin_package()

add_executable(initial_pose_listener src/initial_pose_listener.cpp)
target_link_libraries(initial_pose_listener PUBLIC
    ${catkin_LIBRARIES}
)
target_include_directories(initial_pose_listener PUBLIC
	${catkin_INCLUDE_DIRS}
)

install(TARGETS initial_pose_listener
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
