#define GLFW_INCLUDE_VULKAN
extern "C" {
#include <GLFW/glfw3.h>
}

#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstdlib>

#include <VulkanWindow.hxx>

int main() {
    VulkanWindow *vkWindow = new VulkanWindow(800, 600, "Vulkan Window");

    try {
        while(!glfwWindowShouldClose(vkWindow->window())) {
            glfwPollEvents();
            vkWindow->drawFrame();
            vkWindow->logicalDevice()->waitIdle();
            std::this_thread::sleep_for(std::chrono::milliseconds(17));
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete vkWindow;
    return EXIT_SUCCESS;
}