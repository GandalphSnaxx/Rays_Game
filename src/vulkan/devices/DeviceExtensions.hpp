#pragma once
/**
 * @brief Device extensions for Vulkan.
 * @author Ray Richter
 */
#include <vector>
#include <vulkan/vulkan.h>

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};