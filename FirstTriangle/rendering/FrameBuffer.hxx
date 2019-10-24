#ifndef FRAME_BUFFER_HXX
#define FRAME_BUFFER_HXX

#include <vulkan/vulkan.hpp>

class FrameBuffer {
public:
    FrameBuffer(vk::Device *logicalDevice, vk::ImageView *imageView, vk::Extent2D swapChainExtent, vk::RenderPass *renderPass);
    ~FrameBuffer();

    vk::Framebuffer *buffer();
private:
    vk::Device *m_logicalDevice;
    vk::Framebuffer m_buffer;
};

#endif