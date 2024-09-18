cmake_minimum_required(VERSION 3.20)

project(TCC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/util)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# Add the main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the lib modules to the main executable
target_link_libraries(${PROJECT_NAME}
    Core
    Render
    Util

    GLEW::GLEW
    glfw
    OpenGL::GL
    glm::glm
)

# Include headers dir
target_include_directories(${PROJECT_NAME} PRIVATE
    src/core
    src/render
    src/util
)
