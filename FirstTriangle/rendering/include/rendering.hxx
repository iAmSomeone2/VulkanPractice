#ifndef RENDERING_HXX
#define RENDERING_HXX

// #include "Render.hxx"
// #include "GraphicsPipeline.hxx"
#include <vulkan/vulkan.hpp>

namespace rendering {
    class Render {
    public:
        Render(vk::Device *logicalDevice, vk::Format swapChainImageFormat);
        ~Render();
        vk::RenderPass *renderPass();
    };

    class GraphicsPipeline {
    public:
        GraphicsPipeline(vk::Device *logicalDevice, vk::Extent2D *swapchainExtent, vk::RenderPass *renderPass);
        ~GraphicsPipeline();
    };
};

#endif // RENDERING_HXX