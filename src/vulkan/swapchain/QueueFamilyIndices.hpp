#pragma once
/**
 * @brief Queue family indices struct.
 * @author Ray Richter
 */
#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include "raysDebugHelper.hpp"

struct QueueFamilyIndices { public:
    QueueFamilyIndices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) 
    { VK_CHECK(find(physicalDevice, surface)); }
    QueueFamilyIndices() {}

    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    VkResult find(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) {
        VkResult result;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { graphicsFamily = i; }
            VkBool32 presentSupport = false;
            result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
            if (presentSupport) { presentFamily = i; }
            if (isComplete())   { return result; }
            i++;
        }
        return result;
    }
};