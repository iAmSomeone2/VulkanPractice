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

#include "GraphicsPipeline.hxx"

using namespace vk;

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
    SurfaceCapabilitiesKHR capabilites;
    std::vector<SurfaceFormatKHR> formats;
    std::vector<PresentModeKHR> presentModes;
};

class VulkanWindow {
public:
    VulkanWindow(const uint32_t width, const uint32_t height, const std::string title);
    ~VulkanWindow();

    GLFWwindow *window();
private:
    // Window
    GLFWwindow *m_window;
    uint32_t m_width;
    uint32_t m_height;
    std::string m_title;

    Instance m_instance;
    SurfaceKHR m_surface;

    // Devices
    PhysicalDevice m_device = nullptr;
    Device m_logicalDevice;

    // Queues
    Queue m_graphicsQueue;
    Queue m_presentQueue;

    // Swap chain
    SwapchainKHR m_swapChain;
    std::vector<Image> m_swapChainImages;
    Format m_swapChainImageFormat;
    Extent2D m_swapChainExtent;

    // Graphics
    GraphicsPipeline *m_gPipeline;

    // Image views
    std::vector<ImageView> m_swapChainImageViews;


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
    std::vector<ExtensionProperties> getSupportedExtensions(bool printResults);

    /**
     *  Queries the Vulkan API to confirm that the extensions required by the application are available.
     * 
     * @param device physical device to check
     * @return true if all required extensions are supported.
     */
    bool checkDeviceExtensionSupport(PhysicalDevice device);

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
    bool isDeviceSuitable(PhysicalDevice device);

    /**
     * Determines which queue families are supported by the selected device.
     * 
     * @param device physical device to check
     * @return a struct containing the indices of the queue families
     */
    QueueFamilyIndices findQueueFamilies(PhysicalDevice device);

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
    SwapChainSupportDetails querySwapChainSupport(PhysicalDevice device);

    SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR>& availableFormats);

    PresentModeKHR chooseSwapPresentMode(const std::vector<PresentModeKHR>& availablePresentModes);

    Extent2D chooseSwapExtent(const SurfaceCapabilitiesKHR capabilites);
};

#endif // VULKAN_WINDOW_HXX