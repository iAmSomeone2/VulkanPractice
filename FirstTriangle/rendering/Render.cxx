#include <iostream>

#include "Render.hxx"

Render::Render(vk::Device *logicalDevice, vk::Format swapChainImageFormat) {
    m_logicalDevice = logicalDevice;

    vk::AttachmentDescription colorAttachment({}, swapChainImageFormat, vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics);
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // Create the render pass info and object
    vk::RenderPassCreateInfo renderPassInfo({}, 1, &colorAttachment, 1, &subpass);

    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0U, vk::PipelineStageFlagBits::eColorAttachmentOutput, 
        vk::PipelineStageFlagBits::eColorAttachmentOutput, static_cast<vk::AccessFlags>(0), 
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    try {
        m_logicalDevice->createRenderPass(&renderPassInfo, {}, &m_renderPass, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create render pass." << std::endl;
        throw std::runtime_error(e.what());
    }

}

Render::~Render() {
    m_logicalDevice->destroy(m_renderPass);
}

vk::RenderPass *Render::renderPass() {
    return &m_renderPass;
}