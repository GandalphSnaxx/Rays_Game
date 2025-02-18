#ifndef VK_DEVICES_HPP__
#define VK_DEVICES_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>

#include "vulkan/instance/vk_instance.hpp"
#include "vulkan/surface/vk_surface.hpp"
#include "vulkan/swapchain/vk_swapchain.hpp"
// #include "vulkan/queues/vk_queues.hpp"
#include "vulkan/validation_layers/vk_validation_layers.hpp"
#include "raysDebugHelper.hpp"

/// @brief Queue family indicies
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VkDevicesClass {
    public:

    // Constructor and deconstructor
    VkDevicesClass();
    ~VkDevicesClass();

    VkResult init(VkSurfaceClass *pSurface);
    SwapChainSupportDetails swapchainSupport() { return querySwapChainSupport(physicalDevice); }
    QueueFamilyIndices queueFamilies() { return findQueueFamilies(physicalDevice); }
    VkDevice getDevice() { return device; }
    VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
    VkInstanceClass *getPInstance() { return instance; }
    VkSurfaceClass *getPSurface() { return surface; }

    private:

    /// @brief A list of required extensions
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    // Pointers to other classes
    VkInstanceClass *instance;
    VkSurfaceClass *surface;
    // VkQueuesClass *queues;

    VkResult pickPhysicalDevice();
    VkResult createLogicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice suitableDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice suitableDevice);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice suitableDevice);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice suitableDevice);
};

#endif