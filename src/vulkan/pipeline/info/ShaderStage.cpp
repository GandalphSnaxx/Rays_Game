#include "ShaderStage.hpp"
/**
 * @file
 * @author Ray Richter
 * @brief Shader Stage definitions.
 */

/// @brief Constructs a ShaderStages struct and adds shader files to a VkPipelineShaderStageCreateInfo list.
/// @param shaderFiles A list of shader files to add to ShaderStages
/// @param device Shader stages are created in reference to this device
ShaderStage::ShaderStage(const std::vector<ShaderFile> &shaderFiles, const VkDevice &device) {
    // DEBUG_MSG("Called a ShaderStage struct constructor");
    
    device_ = device; // Copy a pointer to the device for deconstruction
    uint32_t numStages = shaderFiles.size();
    
    // Set the size of the shader stages list
    createInfo_.resize(numStages);
    shaderModules_.resize(numStages);

    // Iterate through the shader files and add initalize them in the shader stages list
    for (uint32_t i = 0; i < numStages; i++) {
        // Create a shader module from the shader files
        // VkShaderModule shaderModule;
        // Initalize shaderModule in reference to device
        VK_CHECK(createShaderModule_(shaderFiles[i], &shaderModules_[i]));

        // Configure the current shader stage struct
        createInfo_[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo_[i].pNext = nullptr; // Optional
        createInfo_[i].flags = 0; // Optional
        createInfo_[i].stage = shaderFiles[i].shaderType;
        createInfo_[i].module = shaderModules_[i];
        createInfo_[i].pName = "main";
        createInfo_[i].pSpecializationInfo = nullptr; // Optional
    }
}

/// @brief Cleans up any data used by this struct
/// TODO: Destroy shader modules after pipeline creation
ShaderStage::~ShaderStage() {
    // DEBUG_MSG("Called a ShaderStage struct deconstructor. This better be called after pipeline creation!");
    for (auto shaderModule : shaderModules_) {
        vkDestroyShaderModule(device_, shaderModule, nullptr);
    }
}

/// @brief Initalizes a shader module from a ShaderFile in reference to a device
/// @param shaderFile The shader file 
/// @param shaderModule A pointer to the shader module to be initalized
/// @return `VkResult`
VkResult ShaderStage::createShaderModule_(const ShaderFile &shaderFile, VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    // Get the shader binary
    auto code = shaderFile.getBinary();
    // Set the configuration for the shader module
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    // Initalize the shader module in reference to a device
    return vkCreateShaderModule(device_, &createInfo, nullptr, shaderModule);
}