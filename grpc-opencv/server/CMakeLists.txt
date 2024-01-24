#
# Dependencies
#
find_package(Threads)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#
# Sources
#
set(SOURCES
    src/video_server.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})


#
# Target
#
add_executable(video_server ${SOURCES})
target_link_libraries(video_server 
    PRIVATE
        myproto
        ${OpenCV_LIBRARIES}
)
