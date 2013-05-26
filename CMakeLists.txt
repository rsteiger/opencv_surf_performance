cmake_minimum_required(VERSION 2.8)
project( OPENCV_SURF_PERFORMANCE )

find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS system filesystem chrono REQUIRED )

include_directories(${Boost_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIR})

add_executable( surftest main.cpp )
target_link_libraries( surftest ${OpenCV_LIBS} ${Boost_LIBRARIES} )
