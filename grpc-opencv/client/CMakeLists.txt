find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCES
    src/video_client.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


#
# Target
#
add_executable(video_client ${SOURCES})
target_link_libraries(video_client
    PRIVATE
        myproto
    ${OpenCV_LIBRARIES}
)
