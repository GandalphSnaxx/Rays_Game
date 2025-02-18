#ifndef VK_SWAPCHAIN_HPP__
#define VK_SWAPCHAIN_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm> // Necessary for std::clamp

#include "vulkan/devices/vk_devices.hpp"
#include "vulkan/surface/vk_surface.hpp"
#include "vulkan/window/vk_window.hpp"
#include "raysDebugHelper.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VkSwapchainClass {
    public:

    // Constructor and deconstructor
    VkSwapchainClass();
    ~VkSwapchainClass();

    VkResult init(VkDevicesClass *pDevice);
    VkResult recreateSwapchain(VkRenderPassClass *renderPass);
    VkFormat getImageFormat() { return swapChainImageFormat; }

    private:

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkDevicesClass *device;
    VkSurfaceClass *surface;
    VkWindowClass *window;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkResult createImageViews();
    VkResult createSwapchain();
    VkResult createFramebuffers(VkRenderPassClass *renderPass);
    void cleanupSwapchain();
};

#endif