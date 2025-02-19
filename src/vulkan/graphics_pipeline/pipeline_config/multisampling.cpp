#include "structs.hpp"

/// @brief Configures multisampling, which is one of the ways to perform anti-aliasing. 
/// It works by combining the fragment shader results of multiple polygons that rasterize to the same pixel.
/// @param rasterizationSamples Number of samples used in rasterization. Set to VK_SAMPLE_COUNT_1_BIT
MultisamplingState::MultisamplingState(const VkSampleCountFlagBits rasterizationSamples) {
    DEBUG_MSG("Called a MultisamplingState struct constructor");
    if (rasterizationSamples != VK_SAMPLE_COUNT_1_BIT) { THROW_ERR("This sample is not supported!"); }
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    createInfo.pNext = nullptr; createInfo.flags = 0; // Optional
    createInfo.rasterizationSamples = rasterizationSamples;
    createInfo.sampleShadingEnable = VK_FALSE;
    createInfo.minSampleShading = 1.0f;
    createInfo.pSampleMask = nullptr;
    createInfo.alphaToCoverageEnable = VK_FALSE;
    createInfo.alphaToOneEnable = VK_FALSE;
}

/// @brief MultisamplingState deconstructor
MultisamplingState::~MultisamplingState() {
    DEBUG_MSG("Called a MultisamplingState struct deconstructor");
}