#pragma once
/**
 * @file vulkan/pipeline/info/MuntisamplingState.hpp
 * @author Ray Richter
 * @brief Multisampling configuration for pipeline creation.
 */
#include <vulkan/vulkan.h>
#include "raysDebugHelper.hpp"

/// @brief A struct for initalizing multisampling info for pipeline creation
struct MultisamplingState {
public:
    MultisamplingState(const VkSampleCountFlagBits &rasterizationSamples = VK_SAMPLE_COUNT_1_BIT);
    ~MultisamplingState();
    // Delete default constructor so it is not accidentally used
    MultisamplingState::MultisamplingState() = delete;

    inline const VkPipelineMultisampleStateCreateInfo* data() const noexcept { return &createInfo_; }
    
private:
    VkPipelineMultisampleStateCreateInfo createInfo_{};

    /// TODO: Check for anti-aliasing support
    // bool sampleRateShadingEnabled_();
    // bool alphaToOneEnabled_();
    // bool mixedSamplesEnabled_();
    // bool aaSupported_();
};