cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 20)

project(hello)

add_executable(${PROJECT_NAME} hello.cpp opencl_utils.cpp)

target_link_libraries(hello OpenCL)
