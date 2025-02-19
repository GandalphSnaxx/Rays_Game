#include "graphics_pipeline.hpp"

/// @brief VkPipelineClass constructor initalizes a graphics pipeline
VkPipelineClass::VkPipelineClass(const std::vector<ShaderFile> &shader_files) :
    // Parent classes and private struct initalizers called in ascending order
    VkPipelineOverheadClass(shader_files),
    VkOverheadClass()
{
    DEBUG_MSG("Called VkPipelineClass constructor");
    VK_CHECK(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &_pipelineInfo, nullptr, &_gfxPipeline));
    _cleanupShaderStages();
}

/// @brief VkPipelineClass deconstructor
VkPipelineClass::~VkPipelineClass() {
    DEBUG_MSG("Called VkPipelineClass deconstructor");
}
