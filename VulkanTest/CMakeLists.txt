cmake_minimum_required(VERSION 3.14)
project(VulkanTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Set up the vulkan-test target
add_executable(vulkan-test main.cxx)

find_package(PkgConfig REQUIRED)
find_package(GLM REQUIRED)
find_package(Vulkan 1.0 REQUIRED)
find_package(Boost 1.29.0 REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

target_include_directories(vulkan-test PRIVATE SYSTEM ${Vulkan_INCLUDE_DIRS})
target_include_directories(vulkan-test PRIVATE SYSTEM ${GLM_INCLUDE_DIRS})
target_include_directories(vulkan-test PRIVATE SYSTEM ${GLFW_INCLUDE_DIRS})
target_include_directories(vulkan-test PRIVATE SYSTEM ${Boost_INCLUDE_DIRS})

target_link_libraries(vulkan-test ${GLFW_LIBRARIES})
target_link_libraries(vulkan-test Vulkan::Vulkan)

add_custom_target(test COMMAND VK_LAYER_PATH=/etc/vulkan/explicit_layer.d ${CMAKE_BINARY_DIR}/vulkan-test)
add_dependencies(test vulkan-test)