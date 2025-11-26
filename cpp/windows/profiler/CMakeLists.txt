cmake_minimum_required(VERSION 4.2.0)
project(WinProfileDemo)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(WinProfileDemo src/main.cpp)
target_link_libraries(WinProfileDemo PRIVATE iphlpapi psapi)
