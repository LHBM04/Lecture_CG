cmake_minimum_required(VERSION 4.0)
project(Lecture_CG LANGUAGES C CXX)

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 0. Warming Up (01 ~ 06)
add_subdirectory("#0_Warming_Up")

# 1. OpenGL Basic (01 ~ 04)
add_subdirectory("#1_OpenGL_Basic_1")