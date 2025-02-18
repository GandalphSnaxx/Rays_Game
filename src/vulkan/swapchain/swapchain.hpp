#ifndef VULKAN_SWAPCHAIN_HPP__
#define VULKAN_SWAPCHAIN_HPP__

#include "overhead.hpp"
#include <vector>
#include <algorithm> // Necessary for std::clamp

/// @brief A class containing variables and functions related to a swapchain that can be used before the creation of a swapchain.
///     Requirements: _surface - initalized
class VkSwapchainOverheadClass : virtual public VkOverheadClass {
    public:
    // No constructor or deconstructor

    protected:
    /// @brief Capabilities, formats, and present mode details for a swapchain
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    VkRenderPass _renderPass;

    SwapchainSupportDetails _querySwapChainSupport(VkPhysicalDevice queriedDevice);
    private:

};

/// @brief A class for handling swapchain related data and functions. A child of VkSwapchainOverheadClass, VkCmdQueueOverheadClass, and grandchild of VkOverheadClass.
///     Device related requirements: _device, _physicalDevice
///     Command queue related requirements: QueueFamilyIndicies struct, _findQueueFamilies().
class VkSwapchainClass : virtual public VkSwapchainOverheadClass {
    public:

    VkSwapchainClass();
    ~VkSwapchainClass();
    // Delete the default constructor to prevent its use
    // VkSwapchainClass::VkSwapchainClass() = delete;

    protected:

    VkResult _recreateSwapchain();

    private:

    VkSwapchainKHR _swapchain;
    std::vector<VkImage> _swapchainImages;
    VkFormat _swapchainImageFormat;
    VkExtent2D _swapchainExtent;
    std::vector<VkImageView> _swapchainImageViews;
    std::vector<VkFramebuffer> _swapchainFramebuffers;
    
    VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkResult _createFramebuffers();
    VkResult _createRenderPass();
    VkResult _createImageViews();
    VkResult _createSwapchain();
    void _cleanupSwapchain();
    void _cleanupRenderPass();
};

#endif