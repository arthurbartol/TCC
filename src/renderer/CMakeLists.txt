# Define the Renderer lib

file (GLOB_RECURSE RENDERER_SOURCES "*.cpp" "*.h")

if (RENDERER_SOURCES)
    add_library(Renderer ${RENDERER_SOURCES})

    target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Renderer PUBLIC GLEW::GLEW glfw OpenGL::GL glm::glm)
endif()
