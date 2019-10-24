#ifndef RENDER_HXX
#define RENDER_HXX

#include <vulkan/vulkan.hpp>

class Render {
public:
    Render(vk::Device *logicalDevice, vk::Format swapChainImageFormat);
    ~Render();
    vk::RenderPass *renderPass();
private:
    vk::Device *m_logicalDevice;
    vk::RenderPass m_renderPass;
};

#endif // RENDER_HXX