cmake_minimum_required(VERSION 2.8.4)
project(CountDownLatch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# threads
find_package(Threads REQUIRED)

add_executable(count_down_latch_example1 CountDownLatch.cpp Example1.cpp)
target_link_libraries(count_down_latch_example1 pthread)

add_executable(count_down_latch_example2 CountDownLatch.cpp Example2.cpp)
target_link_libraries(count_down_latch_example2 pthread)