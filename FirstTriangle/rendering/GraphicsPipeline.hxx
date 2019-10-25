#ifndef GRAPHICS_PIPELINE_HXX
#define GRAPHICS_PIPELINE_HXX

#include <vector>
#include <iostream>
#include <filesystem>
#include <vulkan/vulkan.hpp>

using namespace std::filesystem;
using namespace vk;

class GraphicsPipeline {
public:
    GraphicsPipeline(Device *logicalDevice, Extent2D *swapchainExtent, RenderPass *renderPass);
    ~GraphicsPipeline();
    Pipeline *pipeline();
private:
    Device *m_logicalDevice;
    Extent2D *m_swapchainExtent;
    PipelineLayout m_pipelineLayout;
    Pipeline m_pipeline;

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

    /**
     *  Creates the Pipeline info for both of the shader modules.
     * 
     * @param vertShaderModule Vertex shader module
     * @param fragShaderModule Fragment 
     */
    std::vector<PipelineShaderStageCreateInfo> createShaderStage(ShaderModule vertShaderModule, ShaderModule fragShaderModule);

    PipelineMultisampleStateCreateInfo createMultiSampleStateInfo();
};

#endif // GRAPHICS_PIPELINE_HXX