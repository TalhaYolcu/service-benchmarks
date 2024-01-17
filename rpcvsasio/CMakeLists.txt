cmake_minimum_required(VERSION 3.13)
project(rpc-vs-tcp LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)


find_package(Threads)


set(SOURCES
    rpc/rpc_client.cpp
    rpc/rpc_server.cpp
    tcp/tcp_client.cpp
    tcp/tcp_server.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(rpc_client ${SOURCES})
add_executable(rpc_server ${SOURCES})
add_executable(tcp_client ${SOURCES})
add_executable(tcp_server ${SOURCES})

target_link_libraries(rpc_client
    PRIVATE
        proto
)


