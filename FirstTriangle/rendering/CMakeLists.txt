cmake_minimum_required(VERSION 3.14)
project(FirstTriangle VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_library(rendering STATIC
    GraphicsPipeline.cxx GraphicsPipeline.hxx
    Render.cxx Render.hxx
    FrameBuffer.cxx FrameBuffer.hxx
    VulkanWindow.cxx VulkanWindow.hxx)

set_target_properties(rendering PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(rendering PROPERTIES PUBLIC_HEADER include/rendering.hxx)

find_package(Vulkan 1.0 REQUIRED)

target_include_directories(rendering PRIVATE SYSTEM ${Vulkan_INCLUDE_DIRS})

target_link_libraries(rendering Vulkan::Vulkan)