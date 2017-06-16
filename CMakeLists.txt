cmake_minimum_required(VERSION 2.8.3)
project(trakstar_node)

set(CMAKE_AUTOMOC ON) # Tell CMake to run moc when necessary:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(libatclib_DIR ~/repos/ATC3DGTracker/build)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

link_directories(  ${libatclib_DIR}/lib )
find_library(libatclib libatclib.so ${libatclib_DIR}/lib)


###################################
## catkin specific configuration ##
###################################
catkin_package(
)

###########
## Build ##
###########
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(trakstar src/trakstar.cpp)

target_link_libraries(trakstar
  ${catkin_LIBRARIES}
  ${libatclib}
)
