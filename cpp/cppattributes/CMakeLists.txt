cmake_minimum_required(VERSION 4.2)
project(cppattributes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(cppattributes main.cpp)
