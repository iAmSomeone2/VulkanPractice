#include <fstream>

#include "GraphicsPipeline.hxx"

// TODO: Complete these as I build the pipeline
GraphicsPipeline::GraphicsPipeline(Device *logicalDevice, Extent2D *swapchainExtent, RenderPass *renderPass) {
    m_logicalDevice = logicalDevice;
    m_swapchainExtent = swapchainExtent;

    // Set up our shaders
    auto vertShaderCode = readFile(path("shaders/vert.spv"));
    auto fragShaderCode = readFile(path("shaders/frag.spv"));

    ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // Set up all of the state infos
    auto pipelineShaderInfo = createShaderStage(vertShaderModule, fragShaderModule);
    PipelineVertexInputStateCreateInfo vertexInputInfo({}, 0, nullptr, 0, nullptr);
    PipelineInputAssemblyStateCreateInfo inputAssembly({}, PrimitiveTopology::eTriangleList, false);
    Viewport viewport(0.0f, 0.0f, (float) m_swapchainExtent->width,
        (float) m_swapchainExtent->height, 0.0f, 1.0f);

    // Set up the viewport scissor
    Rect2D scissor = {};
    scissor.offset = Offset2D(0, 0);
    scissor.extent = *m_swapchainExtent;

    // Create the viewport state
    PipelineViewportStateCreateInfo viewportState({}, 1, &viewport, 1, &scissor);

    // Set up the rasterizer
    PipelineRasterizationStateCreateInfo rasterizer({}, false, false, PolygonMode::eFill,
        CullModeFlagBits::eBack, FrontFace::eClockwise, false, 0.0f, 0.0f, 0.0f, 1.0f);

    // Set up multisampling
    PipelineMultisampleStateCreateInfo multisampling({}, SampleCountFlagBits::e1, false, 1.0f, nullptr, false, false);

    //Set up the color blender
    PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = ColorComponentFlagBits::eR | ColorComponentFlagBits::eG | 
        ColorComponentFlagBits::eB | ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = true;
    colorBlendAttachment.srcColorBlendFactor = BlendFactor::eOne;
    colorBlendAttachment.dstColorBlendFactor = BlendFactor::eZero;
    colorBlendAttachment.colorBlendOp = BlendOp::eAdd;
    colorBlendAttachment.srcAlphaBlendFactor = BlendFactor::eOne;
    colorBlendAttachment.dstAlphaBlendFactor = BlendFactor::eZero;
    colorBlendAttachment.alphaBlendOp = BlendOp::eAdd;

    //Set up blend state create info
    PipelineColorBlendStateCreateInfo colorBlending({}, false, LogicOp::eCopy, 1, &colorBlendAttachment);
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // Set up the pipeline layout
    PipelineLayoutCreateInfo pipelineLayoutInfo; // Optional settings have been omitted

    try {
        m_logicalDevice->createPipelineLayout(&pipelineLayoutInfo, {}, &m_pipelineLayout, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create a pipeline layout." << std::endl;
        throw std::runtime_error(e.what());
    }

    GraphicsPipelineCreateInfo pipelineInfo({}, 2, pipelineShaderInfo.data(), &vertexInputInfo,
        &inputAssembly, nullptr, &viewportState, &rasterizer, &multisampling, nullptr, &colorBlending,
        nullptr, m_pipelineLayout, *renderPass);

    try {
        m_logicalDevice->createGraphicsPipelines(nullptr, 1, &pipelineInfo, {}, &m_pipeline, {});
    } catch (std::system_error e) {
        std::cerr << "Failed to create a graphics pipeline." << std::endl;
        throw std::runtime_error(e.what());
    }

    // These are always at the end of this method.
    m_logicalDevice->destroyShaderModule(vertShaderModule);
    m_logicalDevice->destroyShaderModule(fragShaderModule);
};


GraphicsPipeline::~GraphicsPipeline() {
    m_logicalDevice->destroyPipeline(m_pipeline);
    m_logicalDevice->destroyPipelineLayout(m_pipelineLayout);
};

std::vector<char> GraphicsPipeline::readFile(const path& filename) {
    auto fullPath = current_path().append(filename.string());

    std::ifstream file(fullPath.string(), std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

ShaderModule GraphicsPipeline::createShaderModule(const std::vector<char>& code) {
    ShaderModuleCreateInfo createInfo({}, code.size(), reinterpret_cast<const uint32_t*>(code.data()));

    ShaderModule shaderModule;

    m_logicalDevice->createShaderModule(&createInfo, {}, &shaderModule);

    return shaderModule;
}

std::vector<PipelineShaderStageCreateInfo> GraphicsPipeline::createShaderStage(ShaderModule vertShaderModule, ShaderModule fragShaderModule) {
    std::vector<PipelineShaderStageCreateInfo> createInfos(2);

    PipelineShaderStageCreateInfo vertexShaderInfo({}, ShaderStageFlagBits::eVertex,
        vertShaderModule, "main");

    createInfos[0] = vertexShaderInfo;

    PipelineShaderStageCreateInfo fragShaderInfo({}, ShaderStageFlagBits::eFragment,
        fragShaderModule, "main");

    createInfos[1] = fragShaderInfo;

    return createInfos;
}

PipelineMultisampleStateCreateInfo GraphicsPipeline::createMultiSampleStateInfo() {
    VkPipelineMultisampleStateCreateInfo multisampling = {};

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    return multisampling;
}