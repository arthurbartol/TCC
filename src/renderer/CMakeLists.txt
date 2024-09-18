# Define the Renderer lib

file (GLOB_RECURSE RENDERER_SOURCES "*.cpp" "*.h")

add_library(Renderer ${RENDERER_SOURCES})

target_include_directories(Render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Render PUBLIC
    GLEW::GLEW
    glfw
    OpenGL::GL
    glm::glm
)
