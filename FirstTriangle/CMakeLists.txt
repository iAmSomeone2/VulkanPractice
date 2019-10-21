cmake_minimum_required(VERSION 3.14)
project(FirstTriangle VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Set up the triangle target
add_executable(triangle
    main.cxx 
    VulkanWindow.cxx VulkanWindow.hxx
    GraphicsPipeline.cxx GraphicsPipeline.hxx)

find_package(PkgConfig REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan 1.0 REQUIRED)
find_package(Boost 1.29.0 REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

target_include_directories(triangle PRIVATE SYSTEM ${Vulkan_INCLUDE_DIRS})
target_include_directories(triangle PRIVATE SYSTEM ${GLM_INCLUDE_DIRS})
target_include_directories(triangle PRIVATE SYSTEM ${GLFW_INCLUDE_DIRS})
target_include_directories(triangle PRIVATE SYSTEM ${Boost_INCLUDE_DIRS})

target_link_libraries(triangle ${GLFW_LIBRARIES})
target_link_libraries(triangle Vulkan::Vulkan)

add_custom_target(test COMMAND VK_LAYER_PATH=/etc/vulkan/explicit_layer.d ${CMAKE_BINARY_DIR}/triangle)
add_dependencies(test triangle)

# Compile the shaders
add_subdirectory(shaders)