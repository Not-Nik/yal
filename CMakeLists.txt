cmake_minimum_required(VERSION 3.15)
project(yal CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(yal INTERFACE)
target_include_directories(yal INTERFACE .)

add_executable(LoggerTest test/main.cpp)
target_link_libraries(LoggerTest yal)
