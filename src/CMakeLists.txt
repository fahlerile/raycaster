set(
    SOURCES
    main.c
    events.c
    cast.c

    Context.c
    Renderer.c
    Map.c
    Texture.c
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC .)

if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
                         -Wall -Wextra -Wpedantic -Wno-unused -std=c2x)
    if(UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

