#ifndef GRAPHICS_PIPELINE_HXX
#define GRAPHICS_PIPELINE_HXX

#define GLFW_INCLUDE_VULKAN
extern "C" {
#include <GLFW/glfw3.h>
}

#include <vector>
#include <iostream>
#include <filesystem>

using namespace std::filesystem;

class GraphicsPipeline {
public:
    GraphicsPipeline(VkDevice *logicalDevice);
    ~GraphicsPipeline();

private:
    VkDevice *m_logicalDevice;

    /**
     * Reads the binary data from the specified file into a vector.
     * 
     * @param filename path to file
     */
    static std::vector<char> readFile(const path& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    std::vector<VkPipelineShaderStageCreateInfo> createShaderStage(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
};

#endif // GRAPHICS_PIPELINE_HXX