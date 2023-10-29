cmake_minimum_required(VERSION 3.14)
project(hw02)
# require C++20
set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_library(hw02 combinatorics.cpp)
add_executable(runhw02 run.cpp)
target_include_directories(hw02 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(runhw02 hw02)