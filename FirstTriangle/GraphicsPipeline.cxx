#include <fstream>

#include "GraphicsPipeline.hxx"

// TODO: Complete these as I build the pipeline
GraphicsPipeline::GraphicsPipeline(VkDevice *logicalDevice) {
    m_logicalDevice = logicalDevice;

    // Set up our shaders
    auto vertShaderCode = readFile(path("shaders/vert.spv"));
    auto fragShaderCode = readFile(path("shaders/frag.spv"));

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderInfo = createShaderStage(vertShaderModule, fragShaderModule);

    // These are always at the end of this method.
    vkDestroyShaderModule(*m_logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(*m_logicalDevice, vertShaderModule, nullptr);
};


GraphicsPipeline::~GraphicsPipeline() = default;

std::vector<char> GraphicsPipeline::readFile(const path& filename) {


    std::ifstream file(filename, std::ios::ate | std::ios::binary);

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

VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*m_logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module.");
    }

    return shaderModule;
}

std::vector<VkPipelineShaderStageCreateInfo> GraphicsPipeline::createShaderStage(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule) {
    std::vector<VkPipelineShaderStageCreateInfo> createInfos(2);

    for (int i = 0; i < 2; i++) {
        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        // Determine type of shader
        if (i == 0) {
            shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            shaderStageInfo.module = vertShaderModule;
        } else {
            shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            shaderStageInfo.module = fragShaderModule;
        }

        shaderStageInfo.pName = "main";
        createInfos[i] = shaderStageInfo;
    }

    return createInfos;
}