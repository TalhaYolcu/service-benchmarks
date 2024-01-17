#
# Dependencies
#
find_package(Threads)

#
# Sources
#
set(SOURCES
    src/rpc_server.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


#
# Target
#
add_executable(rpc_server ${SOURCES})
target_link_libraries(rpc_server
    PRIVATE
        myproto
)
