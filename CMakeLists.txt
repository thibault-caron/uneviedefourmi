cmake_minimum_required(VERSION 3.31)
project(uneviedefourmi)

set(CMAKE_CXX_STANDARD 14)

include_directories(UneVieDeFourmi)
include_directories(UneVieDeFourmi/fourmilieres)

add_executable(uneviedefourmi
        UneVieDeFourmi/fourmilieres/everything_everywhere.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_3D.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_cinq.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_deux.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_quatre.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_trois.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_un.txt
        UneVieDeFourmi/fourmilieres/fourmiliere_zero.txt
        UneVieDeFourmi/fourmilieres/salle_d_at_ant.txt
        UneVieDeFourmi/src/Ant.cpp
        UneVieDeFourmi/include/Ant.h
        UneVieDeFourmi/src/Anthill.cpp
        UneVieDeFourmi/include/Anthill.h
        UneVieDeFourmi/main.cpp
        UneVieDeFourmi/src/Room.cpp
        UneVieDeFourmi/include/Room.h
        UneVieDeFourmi/include/Path.h)
