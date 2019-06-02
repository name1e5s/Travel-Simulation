cmake_minimum_required(VERSION 3.10)
project(TravelSimulation)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp city_graph.cpp)
include_directories(include)

add_executable(travel ${SOURCE_FILES})
