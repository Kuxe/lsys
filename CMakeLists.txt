project(lsys)
cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

add_compile_options(-std=c++14 -O3 -I.)

add_executable(example lsys.cpp example.cpp)

add_library(lsys SHARED lsys.cpp)

install(TARGETS lsys DESTINATION ${PROJECT_SOURCE_DIR}/install/bin)
install(FILES lsys.hpp DESTINATION ${PROJECT_SOURCE_DIR}/install/include)