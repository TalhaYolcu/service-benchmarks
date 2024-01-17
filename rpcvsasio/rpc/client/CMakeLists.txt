set(SOURCES
    src/rpc_client.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


#
# Target
#
add_executable(rpc_client ${SOURCES})
target_link_libraries(rpc_client
    PRIVATE
        myproto
)
