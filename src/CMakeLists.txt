# CMakeList.txt : CMake project for CakeCutting, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (CakeCutting "main.cpp" "Agent.hpp" "Agent.cpp")

target_include_directories (CakeCutting PRIVATE 
	${CMAKE_SOURCE_DIR}/vendor/eigen
)

# TODO: Add tests and install targets if needed.
