cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(lsys)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

# lsys-target
add_library(lsys SHARED lsys.cpp)
set_target_properties(lsys PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED True
	CXX_EXTENSIONS NO
)
target_include_directories(lsys PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

include(lsys-install)

# Example-target
add_executable(example example.cpp)
set_target_properties(example PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)
target_link_libraries(example PRIVATE lsys)