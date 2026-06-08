cmake_minimum_required(VERSION 3.25)
project(file_converter)

set(CMAKE_CXX_STANDARD 20)

# 1. tell CMake where to find the OpenCV configuration files
set(OpenCV_DIR "C:/opencv/opencv/build")

# 2. find the OpenCV package
find_package(OpenCV REQUIRED)

# 3. include the OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# 4. add cpp files
add_executable(file_converter main.cpp file_converter.cpp)

# 5. link the OpenCV libraries to your executable
target_link_libraries(file_converter ${OpenCV_LIBS})
