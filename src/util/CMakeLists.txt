# Define the Util lib

file(GLOB_RECURSE UTIL_SOURCES "*.cpp" "*.h")

if (UTIL_SOURCES)
    add_library(Util ${UTIL_SOURCES})

    target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Util PUBLIC GLEW::GLEW glfw OpenGL::GL glm::glm)
endif()
