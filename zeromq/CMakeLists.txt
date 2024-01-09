cmake_minimum_required(VERSION 3.0)
project(zeromq_example)

# Locate ZeroMQ package
find_package(PkgConfig)
pkg_check_modules(ZEROMQ libzmqpp)

# Include ZeroMQ headers
include_directories(${ZEROMQ_INCLUDE_DIRS})

# Compile server executable
add_executable(server server.cpp)
target_link_libraries(server ${ZEROMQ_LIBRARIES})

# Compile client executable
add_executable(client client.cpp)
target_link_libraries(client ${ZEROMQ_LIBRARIES})
