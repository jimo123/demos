cmake_minimum_required(VERSION 2.4.8)
project(LOG)

set(Boost_INCLUDE_DIRS "D:\\boost_1_68_0")

include_directories(${Boost_INCLUDE_DIRS} "include")
link_directories("D:/boost_1_68_0/stage/lib")

# add_library(threadsafe_stack_queue threadsafe_stack_queue.cpp)

add_executable(main main.cpp)# threadsafe_stack_queue.cpp)
# target_link_libraries(main threadsafe_stack_queue)
# add_dependencies(main threadsafe_stack_queue)

