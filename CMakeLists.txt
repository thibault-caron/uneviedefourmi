cmake_minimum_required(VERSION 3.31)
project(uneviedefourmi)

set(CMAKE_CXX_STANDARD 14)

include_directories(Test)
include_directories(Test/fourmilieres)

add_executable(uneviedefourmi
        Test/fourmilieres/everything_everywhere.txt
        Test/fourmilieres/fourmiliere_3D.txt
        Test/fourmilieres/fourmiliere_cinq.txt
        Test/fourmilieres/fourmiliere_deux.txt
        Test/fourmilieres/fourmiliere_quatre.txt
        Test/fourmilieres/fourmiliere_trois.txt
        Test/fourmilieres/fourmiliere_un.txt
        Test/fourmilieres/fourmiliere_zero.txt
        Test/fourmilieres/salle_d_at_ant.txt
        Test/Ant.cpp
        Test/Ant.h
        Test/Anthill.cpp
        Test/Anthill.h
        Test/main.cpp
        Test/Room.cpp
        Test/Room.h
        Test/Path.h)
