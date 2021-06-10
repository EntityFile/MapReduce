cmake_minimum_required(VERSION 3.17)
project(MapReduce)

find_package(Threads REQUIRED)
find_package(TBB REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories(${TBB})

add_executable(MapReduce main.cpp map_reduce.cpp map_reduce.h)# map_reduce_tbb.cpp)
target_link_libraries(MapReduce Threads::Threads tbb)