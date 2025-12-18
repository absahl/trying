cmake_minimum_required(VERSION 4.2.0)
project(WinProfileDemo)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(WinProfileDemo src/main.cpp)

target_include_directories(WinProfileDemo PRIVATE ${CMAKE_SOURCE_DIR}/../externals/x64_windows/fmt/include ${CMAKE_SOURCE_DIR}/../externals/x64_windows/spdlog/include)
target_compile_options(WinProfileDemo PRIVATE /utf-8)
target_compile_definitions(WinProfileDemo PRIVATE FMT_SHARED SPDLOG_FMT_EXTERNAL SPDLOG_COMPILED_LIB SPDLOG_SHARED_LIB)
target_link_libraries(WinProfileDemo PRIVATE iphlpapi psapi ${CMAKE_SOURCE_DIR}/../externals/x64_windows/fmt/lib/fmt.lib ${CMAKE_SOURCE_DIR}/../externals/x64_windows/spdlog/lib/spdlog.lib)
