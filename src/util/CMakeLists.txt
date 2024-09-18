# Define the Util lib

file(GLOB_RECURSE UTIL_SOURCES "*.cpp" "*.h")

add_library(Util ${UTIL_SOURCES})

target_include_directories(Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(Util PUBLIC some_dependency(if any))
