cmake_minimum_required(VERSION 3.26)
project(DD1_Project_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Simulator)

add_executable(DD1_Project_1
        Simulator/Circuit.cpp
        Simulator/Circuit.h
        Simulator/Library.cpp
        Simulator/Library.h
        Simulator/Simulator.cpp)
