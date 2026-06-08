cmake_minimum_required(VERSION 3.25)
project(file_converter)

set(CMAKE_CXX_STANDARD 20)

# add cpp files
add_executable(file_converter main.cpp file_converter.cpp)
