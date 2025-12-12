cmake_minimum_required(VERSION 3.10)
project(file_locking_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(file_locking_example main.cpp)
