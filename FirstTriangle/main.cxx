#define GLFW_INCLUDE_VULKAN
extern "C" {
#include <GLFW/glfw3.h>
}

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <VulkanWindow.hxx>

int main() {
    VulkanWindow *vkWindow = new VulkanWindow(800, 600, "Vulkan Window");

    try {
        while(!glfwWindowShouldClose(vkWindow->window())) {
            glfwPollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete vkWindow;
    return EXIT_SUCCESS;
}