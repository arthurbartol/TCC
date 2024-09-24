cmake_minimum_required(VERSION 3.20)

project(TCC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/util)
add_subdirectory(src/shapes)

# Add the main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the lib modules to the main executable
target_link_libraries(${PROJECT_NAME}
    Core
    Renderer
    Util
    Shapes
    GLEW::GLEW
    glfw
    OpenGL::GL
    glm::glm
)

# Include headers dir
target_include_directories(${PROJECT_NAME} PRIVATE
    src/core
    src/renderer
    src/util
    src/shapes
)
