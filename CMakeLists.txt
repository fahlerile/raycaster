cmake_minimum_required(VERSION 3.18.4)
project(raycaster)

set(
  SOURCES
  src/main.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")  # force .exe to build/bin

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC lib/include)
target_include_directories(${PROJECT_NAME} PUBLIC src)

add_library(sdl2 SHARED IMPORTED)
set_property(TARGET sdl2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/SDL2.dll")

# find .dll.a
find_library(
  SDL2_LIB
  SDL2.dll
  HINTS "${CMAKE_SOURCE_DIR}/lib/"
  REQUIRED
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  "${SDL2_LIB}"
)

file(COPY res/ DESTINATION bin/res/)  # copy resources to bin
file(COPY lib/SDL2.dll DESTINATION bin/)  # copy dynamic library to bin
