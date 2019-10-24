#ifndef GRAPHICS_PIPELINE_HXX
#define GRAPHICS_PIPELINE_HXX

#define GLFW_INCLUDE_VULKAN
extern "C" {
#include <GLFW/glfw3.h>
}

#include <vector>
#include <iostream>
#include <filesystem>
#include <vulkan/vulkan.hpp>

using namespace std::filesystem;
using namespace vk;

class GraphicsPipeline {
public:
    GraphicsPipeline(Device *logicalDevice, Extent2D *swapchainExtent);
    ~GraphicsPipeline();

private:
    Device *m_logicalDevice;
    Extent2D *m_swapchainExtent;

    /**
     * Reads the binary data from the specified file into a vector.
     * 
     * @param filename path to file
     */
    static std::vector<char> readFile(const path& filename);

    /**
     * Creates a shader module from the provided shader bytecode.
     * 
     * @param code a vector containing the shader bytecode
     * @return VkShaderModule created from the bytecode
     */
    // VkShaderModule createShaderModule(const std::vector<char>& code);
    ShaderModule createShaderModule(const std::vector<char>& code);

    //std::vector<VkPipelineShaderStageCreateInfo> createShaderStage(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
    std::vector<PipelineShaderStageCreateInfo> createShaderStage(ShaderModule vertShaderModule, ShaderModule fragShaderModule);

    PipelineMultisampleStateCreateInfo createMultiSampleStateInfo();
};

#endif // GRAPHICS_PIPELINE_HXX