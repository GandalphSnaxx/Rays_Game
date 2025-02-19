#include "structs.hpp"

/// @brief Constructs a ShaderStages struct and adds shader files to a VkPipelineShaderStageCreateInfo list.
/// @param shaderFiles A list of shader files to add to ShaderStages
/// @param device Shader stages are created in reference to this device
ShaderStage::ShaderStage(const std::vector<ShaderFile> &shaderFiles, VkDevice device) {
    DEBUG_MSG("Called a ShaderStage struct constructor");
    uint32_t numStages = shaderFiles.size();
    // Set the size of the shader stages list
    createInfo.resize(numStages);

    // Iterate through the shader files and add initalize them in the shader stages list
    for (uint32_t i = 0; i < numStages; i++) {
        // Create a shader module from the shader files
        VkShaderModule shaderModule;
        // Initalize shaderModule in reference to device
        VK_CHECK(_createShaderModule(&shaderFiles[i], device, &shaderModule));

        // Configure the current shader stage struct
        createInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo[i].pNext = nullptr; // Optional
        createInfo[i].flags = 0; // Optional
        createInfo[i].stage = shaderFiles[i].shaderType;
        createInfo[i].module = shaderModule;
        createInfo[i].pName = "main";
        createInfo[i].pSpecializationInfo = nullptr; // Optional
    }
}

/// @brief Cleans up any data used by this struct
ShaderStage::~ShaderStage() {
    DEBUG_MSG("Called a ShaderStage struct deconstructor");
}

/// @brief Initalizes a shader module from a ShaderFile in reference to a device
/// @param shaderFile The shader file 
/// @param device The device the shader module is initalized in reference to
/// @param shaderModule A pointer to the shader module to be initalized
/// @return VkResult
VkResult ShaderStage::_createShaderModule(const ShaderFile *shaderFile, VkDevice device, VkShaderModule *shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    // Get the shader binary
    auto code = shaderFile->getBinary();
    // Set the configuration for the shader module
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    // Initalize the shader module in reference to a device
    return vkCreateShaderModule(device, &createInfo, nullptr, shaderModule);
}