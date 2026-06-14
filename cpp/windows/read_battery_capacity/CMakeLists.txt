cmake_minimum_required(VERSION 4.2)
project(read_battery_capacity)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# setting and configuring targets
add_executable(read_battery_capacity_synchronously src/synchronous.cpp)
add_executable(read_battery_capacity_asynchronously src/asynchronous.cpp)
add_definitions(-DUNICODE -D_UNICODE)
target_link_libraries(read_battery_capacity_synchronously PRIVATE SetupAPI)
target_link_libraries(read_battery_capacity_asynchronously PRIVATE SetupAPI)
