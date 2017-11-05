project(lsystem)
cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

add_compile_options(-std=c++14 -O3 -I.)

add_executable(example lsys.cpp example.cpp)

add_library(lsys SHARED lsys.cpp)