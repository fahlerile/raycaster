add_subdirectory(c-codebase)

target_link_libraries(
    ${EXECUTABLE_NAME}
    Color
    Vector
    Matrix
    DynamicArray
    fileUtils
    memoryUtils
    stringUtils
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    SDL2 SDL2main
)

if(UNIX)
    target_link_libraries(
        ${EXECUTABLE_NAME}
        m
    )
endif()

target_include_directories(
    ${EXECUTABLE_NAME} PUBLIC
    c-codebase
    include
)

