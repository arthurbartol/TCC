# Define the Shapes lib

file(GLOB_RECURSE SHAPES_SOURCES "*.cpp" "*.h")

if (SHAPES_SOURCES)
    add_library(Shapes ${SHAPES_SOURCES})

    target_include_directories(Shapes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(Shapes PUBLIC ${CMAKE_SOURCE_DIR}/src/shapes)
endif()
