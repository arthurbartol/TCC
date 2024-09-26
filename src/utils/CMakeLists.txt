# Define the Util lib

file(GLOB_RECURSE UTILS_SOURCES "*.cpp" "*.h")

if (UTILS_SOURCES)
    add_library(Util ${UTILS_SOURCES})

    target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Utils PUBLIC GLEW::GLEW glfw OpenGL::GL glm::glm)
endif()
