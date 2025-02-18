#ifndef VULKAN_RENDER_PASS_HPP__
#define VULKAN_RENDER_PASS_HPP__

#include "overhead.hpp"

/// @brief A class for handling render pass related variables and functions
///     Requires access to: _device, _swapchainImageFormat
class VkRenderPassClass : virtual public VkOverheadClass {
    public:
    VkRenderPassClass();
    ~VkRenderPassClass();
    // Delete the default constructor to prevent its use
    // VkRenderPassClass::VkRenderPassClass() = delete;

    protected:

    VkResult _recreateSwapchain();

    private:

    VkRenderPass _renderPass;
    VkResult _createFramebuffers();
};

#endif