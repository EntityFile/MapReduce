cmake_minimum_required(VERSION 3.17)
project(MapReduce)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(MapReduce main.cpp map_reduce.cpp map_reduce.cpp map_reduce.h)
target_link_libraries(MapReduce Threads::Threads)