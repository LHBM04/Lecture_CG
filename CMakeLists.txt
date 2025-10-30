cmake_minimum_required(VERSION 3.26)
project(Lecture_CG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Lv.0 [Warming Up (01 ~ 06)]
add_subdirectory("Level_00")

# Lv.1 [OpenGL Basic (01 ~ 12)]
add_subdirectory("Level_01")