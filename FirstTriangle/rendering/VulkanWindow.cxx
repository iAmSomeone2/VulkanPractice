#include <boost/format.hpp>

#include <stdexcept>
#include <set>
#include <cstdint>
#include <cctype>

#include "VulkanWindow.hxx"

// ----- Public Methods -----

VulkanWindow::VulkanWindow(const uint32_t width, const uint32_t height, const std::string title) {
    // Set instance variables
    if (width > 0) {
        m_width = width;
    } else {
        m_width = DEFAULT_WIDTH;
    }

    if (height > 0) {
        m_height = height;
    } else {
        m_height = DEFAULT_HEIGHT;
    }

    m_title = title;

    // Set up GLFW window.
    this->initWindow();

    // Set up Vulkan instance
    this->initVulkan();

};

VulkanWindow::~VulkanWindow() {
    // Destroy the FrameBuffers
    for (auto buffer : m_frameBuffers) {
        delete buffer;
    }

    // Destroy the graphics pipeline
    delete m_gPipeline;
    delete m_render;

    // Destroy our image views
    for (auto imageView : m_swapChainImageViews) {
        m_logicalDevice.destroyImageView(imageView);
    }

    m_logicalDevice.destroySwapchainKHR(m_swapChain);
    m_logicalDevice.destroy();
    m_instance.destroySurfaceKHR(m_surface);
    m_instance.destroy();

    glfwDestroyWindow(m_window);

    glfwTerminate();
};

GLFWwindow *VulkanWindow::window() {
    return m_window;
}

// ----- Private Methods -----
// ----- Instance Management -----

void VulkanWindow::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
}

void VulkanWindow::initVulkan() {
    createVkInstance();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    m_render = new Render(&m_logicalDevice, m_swapChainImageFormat);
    m_gPipeline = new GraphicsPipeline(&m_logicalDevice, &m_swapChainExtent, m_render->renderPass());
    createFrameBuffers();
git     std::cout << "I'm just curious..." << std::endl;
}

// ----- Vulkan-specific methods -----

void VulkanWindow::createVkInstance() {
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    vk::ApplicationInfo info("Vulkan Triangle", VK_MAKE_VERSION(1, 0, 0),
        "No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);


    vk::InstanceCreateInfo create({}, &info);

    if (ENABLE_VALIDATION_LAYERS) {
        create.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create.enabledLayerCount = 0;
    }

    // Figure out which extensions we have access to
    uint32_t glfwExtCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtCount);

    create.enabledExtensionCount = glfwExtCount;
    create.ppEnabledExtensionNames = glfwExtensions;
    
    vk::createInstance(&create, nullptr, &m_instance);

    // getSupportedExtensions(true);
}

void VulkanWindow::createSurface() {
    if (glfwCreateWindowSurface(m_instance, m_window, nullptr, (VkSurfaceKHR *) &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface.");
    }
}

void VulkanWindow::createImageViews() {
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
            vk::ImageViewCreateInfo createInfo({}, m_swapChainImages[i], 
            vk::ImageViewType::e2D, m_swapChainImageFormat);

        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;

        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor; 
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        try {
            m_logicalDevice.createImageView(&createInfo, {}, &m_swapChainImageViews[i]);
        } catch (std::system_error e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

void VulkanWindow::createFrameBuffers() {
    m_frameBuffers.resize(m_swapChainImageViews.size());
    for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
        vk::ImageView attachment = m_swapChainImageViews[i];

        m_frameBuffers[i] = new FrameBuffer(&m_logicalDevice, &attachment, m_swapChainExtent, m_render->renderPass());
    }
}

bool VulkanWindow::checkValidationLayerSupport() {
    uint32_t layerCount;
    vk::enumerateInstanceLayerProperties(&layerCount, {}, {});

    std::vector<vk::LayerProperties> availableLayers(layerCount);
    vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data(), {});

    for (const std::string layerName : VALIDATION_LAYERS) {
        bool layerFound = false;

        for (const auto& layerProps : availableLayers) {
            if (layerName == layerProps.layerName) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<vk::ExtensionProperties> VulkanWindow::getSupportedExtensions(bool printResults) {
    uint32_t extCount = 0;
    vk::enumerateInstanceExtensionProperties({}, &extCount, {}, {});

    std::vector<vk::ExtensionProperties> extensions(extCount);
    vk::enumerateInstanceExtensionProperties({}, &extCount, extensions.data(), {});

    if (printResults) {
        std::clog << "Available extensions:" << std::endl;

        for (const auto& extension : extensions) {
            std::clog << boost::format("\t%s") % extension.extensionName << std::endl;
        }
    }

    return extensions;
}

bool VulkanWindow::checkDeviceExtensionSupport(vk::PhysicalDevice device) {
    uint32_t extCount = 0;
    device.enumerateDeviceExtensionProperties({}, &extCount, {}, {});

    std::vector<vk::ExtensionProperties> availableExtensions(extCount);
    device.enumerateDeviceExtensionProperties({}, &extCount, availableExtensions.data(), {});

    std::set<std::string> requiredExts(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

    for (const auto& extension : availableExtensions) {
        requiredExts.erase(extension.extensionName);
    }

    return requiredExts.empty();
}

void VulkanWindow::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    m_instance.enumeratePhysicalDevices(&deviceCount, {}, {});

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find any devices with Vulkan support!");
    }

    std::vector<vk::PhysicalDevice> devices(deviceCount);
    m_instance.enumeratePhysicalDevices(&deviceCount, devices.data(), {});

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_device = device;
            break;
        }
    }
}

bool VulkanWindow::isDeviceSuitable(vk::PhysicalDevice device) {
    vk::PhysicalDeviceProperties deviceProps;
    deviceProps = device.getProperties();

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures = device.getFeatures();

    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return deviceFeatures.geometryShader && indices.isComplete() && extensionsSupported
        && swapChainAdequate;
}

QueueFamilyIndices VulkanWindow::findQueueFamilies(vk::PhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    device.getQueueFamilyProperties(&queueFamilyCount, {}, {});

    std::vector<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
    device.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.data(), {});

    uint32_t i = 0;
    for (const auto& family : queueFamilies) {
        vk::Bool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
        device.getSurfaceSupportKHR(i, m_surface, &presentSupport, {});

        if (family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        if (family.queueCount > 0 && presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void VulkanWindow::createLogicalDevice() {
    // Set up queue features
    QueueFamilyIndices indices = findQueueFamilies(m_device);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);

        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Specify device features
    vk::PhysicalDeviceFeatures deviceFeatures = {};

    // Create the logical device
    vk::DeviceCreateInfo createInfo({}, static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    try {
        m_device.createDevice(&createInfo, {}, &m_logicalDevice, {});

        m_logicalDevice.getQueue(indices.presentFamily.value(), 0, &m_presentQueue, {});
        m_logicalDevice.getQueue(indices.graphicsFamily.value(), 0, &m_graphicsQueue, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create a logical Vulkan device." << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void VulkanWindow::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_device);

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilites);

    // Determine max number of images to render
    uint32_t imageCount = swapChainSupport.capabilites.minImageCount + 1;
    if (swapChainSupport.capabilites.maxImageCount > 0 && imageCount > swapChainSupport.capabilites.maxImageCount) {
        imageCount = swapChainSupport.capabilites.maxImageCount;
    }

    // Create the swap chain object
    vk::SwapchainCreateInfoKHR createInfo({}, m_surface, imageCount, surfaceFormat.format,
        surfaceFormat.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment);

    QueueFamilyIndices indices = findQueueFamilies(m_device);
    uint32_t queueFamilyIndices[] = {
        indices.graphicsFamily.value(),
        indices.presentFamily.value()
    };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = {};
    }

    createInfo.preTransform = swapChainSupport.capabilites.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

    createInfo.presentMode = presentMode;
    createInfo.clipped = true;

    try {
        m_logicalDevice.createSwapchainKHR(&createInfo, {}, &m_swapChain, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create swapchain." <<std::endl;
        throw std::runtime_error(e.what());
    }

    // Retrieve the swap chain images
    m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &imageCount, {}, {});
    m_swapChainImages.resize(imageCount);
    m_logicalDevice.getSwapchainImagesKHR(m_swapChain, &imageCount, m_swapChainImages.data(), {});

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

SwapChainSupportDetails VulkanWindow::querySwapChainSupport(vk::PhysicalDevice device){
    SwapChainSupportDetails details;
    device.getSurfaceCapabilitiesKHR(m_surface, &details.capabilites, {});

    uint32_t formatCount;
    device.getSurfaceFormatsKHR(m_surface, &formatCount, {}, {});

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        device.getSurfaceFormatsKHR(m_surface, &formatCount, details.formats.data(), {});
    }

    uint32_t presentModeCount;
    device.getSurfacePresentModesKHR(m_surface, &presentModeCount, {}, {});

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        device.getSurfacePresentModesKHR(m_surface, &presentModeCount, details.presentModes.data(), {});
    }

    return details;
}

vk::SurfaceFormatKHR VulkanWindow::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    // Check for sRGB support
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR VulkanWindow::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D VulkanWindow::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilites) {
    if (capabilites.currentExtent.width != UINT32_MAX) {
        return capabilites.currentExtent;
    } else {
        vk::Extent2D actualExtent = {m_width, m_height};

        actualExtent.width = std::max(capabilites.minImageExtent.width, std::min(capabilites.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilites.minImageExtent.height, std::min(capabilites.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}