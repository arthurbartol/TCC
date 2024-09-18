add_library(Render

)

target_include_directories(Render PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Render PUBLIC
    GLEW::GLEW
    glfw
    OpenGL::GL
    glm::glm
)
