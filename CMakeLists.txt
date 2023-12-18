cmake_minimum_required(VERSION 3.18.4)
project(raycaster)

set(EXECUTABLE_NAME raycaster)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_compile_definitions(NO_LOG_MEMORY)

add_subdirectory(src)
add_subdirectory(lib)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res/)

