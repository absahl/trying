cmake_minimum_required(VERSION 4.2)
project(battery_health_monitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# setting source files
set (SOURCES src/main.cpp)

# setting and configuring target
add_executable(battery_health_monitor ${SOURCES})
add_definitions(-DUNICODE -D_UNICODE)
