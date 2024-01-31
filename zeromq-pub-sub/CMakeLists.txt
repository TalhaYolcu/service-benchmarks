cmake_minimum_required(VERSION 3.0)
project(zeromq_example)

# Locate ZeroMQ package
find_package(PkgConfig)
pkg_check_modules(ZEROMQ libzmq czmq)

# Include ZeroMQ headers
include_directories(${ZEROMQ_INCLUDE_DIRS})

# Compile server executable
add_executable(pub pub.c)
target_link_libraries(pub ${ZEROMQ_LIBRARIES})

# Compile client executable
add_executable(sub sub.c)
target_link_libraries(sub ${ZEROMQ_LIBRARIES})
