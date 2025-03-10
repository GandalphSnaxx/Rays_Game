#include "Layout.hpp"
/**
 * @file vulkan/pipeline/info/Layout.cpp
 * @author Ray Richter
 * @brief Pipeline layout config for making a Vulkan graphics pipeline.
 */

Layout::Layout(
const VkDevice &device,
VkPipelineLayout *pPipelineLayout,
const std::vector<VkDescriptorSetLayout> &descriptorSetLayouts,
const VkAllocationCallbacks *pAllocator) {
    // DEBUG_MSG("Called Layout constructor");

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount           = descriptorSetLayouts.size();
    createInfo.pSetLayouts              = (descriptorSetLayouts.size() == 0) ? nullptr : descriptorSetLayouts.data();
    createInfo.pushConstantRangeCount   = 0; // Optional
    createInfo.pPushConstantRanges      = nullptr; // Optional

    VK_CHECK(vkCreatePipelineLayout(device, &createInfo, pAllocator, pPipelineLayout));
}

Layout::~Layout() {
    // DEBUG_MSG("Called Layout deconstructor");
}