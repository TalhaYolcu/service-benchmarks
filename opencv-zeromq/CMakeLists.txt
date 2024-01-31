cmake_minimum_required(VERSION 3.0)
project(zeromq_example)

# Locate ZeroMQ package
find_package(PkgConfig)
pkg_check_modules(ZEROMQ libzmqpp)

# Locate OpenCV package
find_package(OpenCV REQUIRED)

# Include directories for ZeroMQ and OpenCV
include_directories(${ZEROMQ_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

# Compile server executable
add_executable(video_server video_server.cpp)
target_link_libraries(video_server ${ZEROMQ_LIBRARIES} ${OpenCV_LIBRARIES})

# Compile client executable
add_executable(video_client video_client.cpp)
target_link_libraries(video_client ${ZEROMQ_LIBRARIES} ${OpenCV_LIBRARIES})

