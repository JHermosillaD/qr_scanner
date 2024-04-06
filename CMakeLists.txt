cmake_minimum_required(VERSION 3.0.2)
project(qr_scanner)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  message_generation
)

###################################
## catkin specific configuration ##
###################################

catkin_package(
  CATKIN_DEPENDS roscpp sensor_msgs cv_bridge image_transport
)

###########
## Build ##
###########

find_library(ZBAR_LIBRARIES NAMES zbar)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(activate_node src/main.cpp)
target_link_libraries(activate_node zbar ${catkin_LIBRARIES})