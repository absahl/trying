cmake_minimum_required(VERSION 4.2)
project(read_battery_capacity)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# setting source files
set (SOURCES src/main.cpp)

# setting and configuring target
add_executable(read_battery_capacity ${SOURCES})
add_definitions(-DUNICODE -D_UNICODE)
target_link_libraries(read_battery_capacity PRIVATE SetupAPI)
