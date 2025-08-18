cmake_minimum_required(VERSION 3.10)
project(FileLockExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(file_lock_fork file_lock_fork.cpp)
