cmake_minimum_required(VERSION 2.4.8)
project(opencv_test)

set(OpenCV_INCLUDE_DIRS "D:\\opencv342\\build\\install\\include")
set(OpenCV_LIBS "D:\\opencv342\\build\\install\\x64\\vc12\\lib\\opencv_world342d.lib" "D:\\opencv342\\build\\install\\x64\\vc12\\lib\\opencv_world342.lib")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})

add_executable(main_advance main_advance.cpp)
target_link_libraries(main_advance ${OpenCV_LIBS})
