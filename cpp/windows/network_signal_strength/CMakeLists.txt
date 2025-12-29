cmake_minimum_required(VERSION 4.2)
project(network_signal_strength)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# setting source files
set (SOURCES src/main.cpp)

# setting and configuring target
add_executable(network_signal_strength ${SOURCES})
target_compile_definitions(network_signal_strength PRIVATE -DUNICODE -D_UNICODE)
target_link_libraries(network_signal_strength PRIVATE wlanapi ole32)
