#include "FrameBuffer.hxx"

#include <iostream>

FrameBuffer::FrameBuffer(vk::Device *logicalDevice, vk::ImageView *imageView, vk::Extent2D swapChainExtent, vk::RenderPass *renderPass) {
    m_logicalDevice = logicalDevice;

    vk::FramebufferCreateInfo createInfo({}, *renderPass, 1, imageView,
        swapChainExtent.width, swapChainExtent.height, 1);

    try {
        m_logicalDevice->createFramebuffer(&createInfo, {}, &m_buffer, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create framebuffer." << std::endl;
        throw std::runtime_error(e.what());
    }
}

FrameBuffer::~FrameBuffer() {
    m_logicalDevice->destroyFramebuffer(m_buffer);
}

vk::Framebuffer *FrameBuffer::buffer() {
    return &m_buffer;
}