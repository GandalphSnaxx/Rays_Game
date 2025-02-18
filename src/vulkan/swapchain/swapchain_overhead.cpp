#include "swapchain.hpp"

/// @brief Returns the swapchain support details of a queried VkPhysicalDevice
/// @param queriedDevice The device to be queried
/// @return Swapchain support details
VkSwapchainOverheadClass::SwapchainSupportDetails VkSwapchainOverheadClass::_querySwapChainSupport(VkPhysicalDevice queriedDevice) {
    SwapchainSupportDetails details;

    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(queriedDevice, _surface, &details.capabilities));

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(queriedDevice, _surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(queriedDevice, _surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(queriedDevice, _surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(queriedDevice, _surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}