cmake_minimum_required(VERSION 3.31)
project(uneviedefourmi)

set(CMAKE_CXX_STANDARD 14)

include_directories(UneVieDeFourmi)
include_directories(UneVieDeFourmi/fourmilieres)

add_executable(uneviedefourmi
        UneVieDeFourmi/src/Ant.cpp
        UneVieDeFourmi/src/Anthill.cpp
        UneVieDeFourmi/main.cpp
        UneVieDeFourmi/src/Room.cpp)
