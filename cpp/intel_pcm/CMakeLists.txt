cmake_minimum_required(VERSION 4.2)
project(intel_pcm_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTOR ${CMAKE_SOURCE_DIR}/bin)

# setting source files
set (PCM C:/Users/Hp/source/repos/pcm)
set (PCM_SRC ${PCM}/src)
set (SOURCES main.cpp ${PCM_SRC}/cpucounters.cpp ${PCM_SRC}/msr.cpp ${PCM_SRC}/pci.cpp ${PCM_SRC}/topology.cpp ${PCM_SRC}/utils.cpp)

# setting and configuring target
add_executable(intel_pcm_example ${SOURCES})
target_include_directories(intel_pcm_example PRIVATE ${PCM_SRC})
target_compile_definitions(intel_pcm_example PRIVATE UNICODE _UNICODE)
set_target_properties(intel_pcm_example PROPERTIES CHARACTER_SET UNICODE)
