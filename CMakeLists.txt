cmake_minimum_required(VERSION 2.8.4)
project(CountDownLatch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")

# threads
find_package(Threads REQUIRED)

# Generate libCountDownLatch.so file
ADD_LIBRARY(CountDownLatch SHARED CountDownLatch.cpp)

add_executable(count_down_latch_example1 Example1.cpp)
target_link_libraries(count_down_latch_example1 CountDownLatch pthread)

add_executable(count_down_latch_example2 Example2.cpp)
target_link_libraries(count_down_latch_example2 CountDownLatch pthread)
