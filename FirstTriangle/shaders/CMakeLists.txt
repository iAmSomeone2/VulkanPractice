cmake_minimum_required(VERSION 3.14)
project(FirstTriangle VERSION 1.0 LANGUAGES CXX)

find_program(GLSLC glslc HINTS /usr/bin)
message(STATUS "glslc located at: ${GLSLC}")

add_custom_target(vertshader
    COMMAND ${GLSLC} ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert -o ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
    SOURCES shader.vert
    COMMENT "Compile vertex shader")

add_custom_target(fragshader
    COMMAND ${GLSLC} ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag -o ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
    SOURCES shader.frag
    COMMENT "Compile fragment shader")

add_custom_target(shaders ALL)
add_dependencies(shaders vertshader fragshader)