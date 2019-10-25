#ifndef VULKAN_WINDOW_HXX
#define VULKAN_WINDOW_HXX

#define GLFW_INCLUDE_VULKAN
extern "C" {
#include <GLFW/glfw3.h>
}

#include <iostream>
#include <vector>
#include <optional>
#include <vulkan/vulkan.hpp>

#include "Render.hxx"
#include "GraphicsPipeline.hxx"
#include "FrameBuffer.hxx"

static const uint32_t DEFAULT_WIDTH = 800;
static const uint32_t DEFAULT_HEIGHT = 600;

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    static const bool ENABLE_VALIDATION_LAYERS = false;
#else
    static const bool ENABLE_VALIDATION_LAYERS = true;
#endif // NDEBUG

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilites;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class VulkanWindow {
public:
    VulkanWindow(const uint32_t width, const uint32_t height, const std::string title);
    ~VulkanWindow();

    GLFWwindow *window();
    vk::Device *logicalDevice();
    void drawFrame();
private:
    // Window
    GLFWwindow *m_window;
    uint32_t m_width;
    uint32_t m_height;
    std::string m_title;

    vk::Instance m_instance;
    vk::SurfaceKHR m_surface;

    // Devices
    vk::PhysicalDevice m_device = nullptr;
    vk::Device m_logicalDevice;

    // Queues
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;

    // Swap chain
    vk::SwapchainKHR m_swapChain;
    std::vector<vk::Image> m_swapChainImages;
    vk::Format m_swapChainImageFormat;
    vk::Extent2D m_swapChainExtent;

    // Graphics
    GraphicsPipeline *m_gPipeline;
    Render *m_render;
    std::vector<FrameBuffer*> m_frameBuffers;
    vk::CommandPool m_commandPool;
    std::vector<vk::CommandBuffer> m_commandBuffers;

    // Image views
    std::vector<vk::ImageView> m_swapChainImageViews;

    // Drawing
    vk::Semaphore m_imgAvailableSemaphore;
    vk::Semaphore m_renderFinishedSemaphore;


    // -----Instance management methods-----

    /**
     * Initializes the GLFW window.
     */
    void initWindow();

    /**
     * Uses the Vulkan-specific methods to initialize the Vulkan pipeline.
     */
    void initVulkan();

    // -----Vukan-specific methods-----

    /**
     * Creates an instance of the Vulkan pipeline.
     */
    void createVkInstance();

    /**
     * Creates an instance surface for Vulkan to draw to.
     */
    void createSurface();

    void createImageViews();

    /**
     * Sets up the swapchain
     */
    void createFrameBuffers();

    /**
     * Sets up the command pool.
     */
    void createCommandPool();

    /**
     * Allocates and creates the vector containing the command buffers.
     */
    void createCommandBuffers();

    void createSemaphores();

    /**
     * Checks if the required validation layers are present on the machine.
     * 
     * This is only used in debug builds.
     * @return true if all required validation layers are present
     */
    bool checkValidationLayerSupport();

    /**
     * Queries the Vulkan API for a list of extensions that the host machine supports.
     * 
     * @param printResults If set to "true", the method will print the list of available extensions to the standard log.
     * @return a vector containing the Vulkan extensions supported by the host machine.
     */
    std::vector<vk::ExtensionProperties> getSupportedExtensions(bool printResults);

    /**
     *  Queries the Vulkan API to confirm that the extensions required by the application are available.
     * 
     * @param device physical device to check
     * @return true if all required extensions are supported.
     */
    bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

    /**
     * Queries the Vulkan API for a list of graphics-capable devices on the host machine and selects the most appropriate one.
     */
    void pickPhysicalDevice();

    /**
     * Determines if a device is suitable for use with Vulkan.
     * 
     * @param device physical device to check
     * @return true if the device is suitable
     */
    bool isDeviceSuitable(vk::PhysicalDevice device);

    /**
     * Determines which queue families are supported by the selected device.
     * 
     * @param device physical device to check
     * @return a struct containing the indices of the queue families
     */
    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

    /**
     * Creates a logical Vulkan device to relay commands to the GPU.
     */
    void createLogicalDevice();

    /**
     * Creates a swap chain for the application to draw to.
     */
    void createSwapChain();

    /**
     * Retrieves the Swap Chain support details of the device
     * 
     * @param device physical device to check
     * @return a SwapChainSupportDetails struct
     */
    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilites);
};

#endif // VULKAN_WINDOW_HXX