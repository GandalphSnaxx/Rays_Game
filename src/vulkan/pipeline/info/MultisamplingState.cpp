#include "MultisamplingState.hpp"
/**
 * @file vulkan/pipeline/info/MultisamplingState.cpp
 * @author Ray Richter
 * @brief Multisampling config for pipeline creation.
 */

/// @brief Configures multisampling, which is one of the ways to perform anti-aliasing. 
/// It works by combining the fragment shader results of multiple polygons that rasterize to the same pixel.
/// @param rasterizationSamples Number of samples used in rasterization. Set to VK_SAMPLE_COUNT_1_BIT
MultisamplingState::MultisamplingState(const VkSampleCountFlagBits &rasterizationSamples = VK_SAMPLE_COUNT_1_BIT) {
    // DEBUG_MSG("Called a MultisamplingState struct constructor");

    /// TODO: Add other sample count support
    if (rasterizationSamples != VK_SAMPLE_COUNT_1_BIT) { 
        THROW_ERR("MultisamplingState ERROR: This sample is not supported (yet...)!"); }
    createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    createInfo_.pNext = nullptr; createInfo_.flags = 0; // Optional

    createInfo_.rasterizationSamples    = rasterizationSamples;
    createInfo_.sampleShadingEnable     = VK_FALSE;
    createInfo_.minSampleShading        = 1.0f;
    createInfo_.pSampleMask             = nullptr;
    createInfo_.alphaToCoverageEnable   = VK_FALSE;
    createInfo_.alphaToOneEnable        = VK_FALSE;
}

/// @brief MultisamplingState deconstructor
MultisamplingState::~MultisamplingState() {
    // DEBUG_MSG("Called a MultisamplingState struct deconstructor");
}