#pragma once
/**
 * @file vulkan/pipeline/info/Layout.hpp
 * @author Ray Richter
 * @brief Pipeline layout config for creating a graphics pipeline for Vulkan.
 */
#include <vulkan/vulkan.h>
#include <vector>
#include "raysDebugHelper.hpp"

struct Layout {
public:
    /// @brief Initalizes a graphics pipeline layout and attaches it to a device.
    /// @param device The device to add a pipeline to.
    /// @param descriptorSetLayouts A list of descriptor set layouts to add to the pipeline. Default to none
    /// @param pAllocator Pointer to a Vulkan Allocation Callback. Default to nullptr.
    /// @param pPipelineLayout Pointer to Vulkan Pipeline Layout
    Layout(
        const VkDevice &device,
        VkPipelineLayout *pPipelineLayout,
        const std::vector<VkDescriptorSetLayout> &descriptorSetLayouts = {},
        const VkAllocationCallbacks *pAllocator = nullptr);
    ~Layout();
    // Delete the default constructor so it is not accidentally used
    Layout::Layout() = delete;

    // operator VkPipelineLayoutCreateInfo() const { return createInfo; }
    // VkPipelineLayoutCreateInfo* info() const { return &createInfo; }

    VkPipelineLayoutCreateInfo createInfo{};
private:
};