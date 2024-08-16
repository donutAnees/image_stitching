cmake_minimum_required(VERSION 3.0)
project( Sticher )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Stitcher source/main.cpp source/stitcher.cpp source/imageproc.cpp)
#[add_library( libstitcher SHARED source/main.cpp source/stitcher.cpp source/imageproc.cpp)]#
target_link_libraries(  Stitcher ${OpenCV_LIBS} )