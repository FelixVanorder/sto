cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_executable( test_sto "test/test_sto.cpp" )
add_executable( test_benchmark_sto "test/test_benchmark_sto.cpp" )

target_include_directories( test_sto PUBLIC ./include/ )
target_include_directories( test_benchmark_sto PUBLIC ./include/ )

