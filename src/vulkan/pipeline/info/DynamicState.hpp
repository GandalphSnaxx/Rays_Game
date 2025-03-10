#pragma once
/**
 * @file
 * @author Ray Richter
 * @brief Dynamic state and viewport create info struct for pipeline creation.
 */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <FlagField.hpp>

const std::vector<VkDynamicState> DYNAMIC_STATES = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR};

/// @brief A struct holding VkPipelineDynamicStateCreateInfo related info for pipeline creation
struct DynamicStateStage {
public:
    DynamicStateStage (const std::vector<VkDynamicState> &stages = DYNAMIC_STATES);
    ~DynamicStateStage();
    // Delete default constructor so it is not accidentally used
    DynamicStateStage::DynamicStateStage() = delete;

    inline const VkPipelineDynamicStateCreateInfo*  dynamicData () const noexcept { return &dynamicCreateInfo_;  }
    inline const VkPipelineViewportStateCreateInfo* viewportData() const noexcept { return &viewportCreateInfo_; }

private:
    VkPipelineDynamicStateCreateInfo    dynamicCreateInfo_{};
    VkPipelineViewportStateCreateInfo   viewportCreateInfo_{};
};