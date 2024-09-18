# Define the Core lib

file(GLOB_RECURSE CORE_SOURCES "*.cpp" "*.h")

if (CORE_SOURCES)
    add_library(Core ${CORE_SOURCES})

    target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Core PUBLIC GLEW::GLEW glfw OpenGL::GL glm::glm)
endif()
