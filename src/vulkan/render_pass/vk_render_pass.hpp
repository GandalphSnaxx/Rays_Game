#ifndef VK_RENDER_PASS_HPP__
#define VK_RENDER_PASS_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/devices/vk_devices.hpp"

class VkRenderPassClass {
    public:

    // Constructor and deconstructor
    VkRenderPassClass();
    ~VkRenderPassClass();

    VkResult init(VkDevicesClass *pDevice, VkSwapchainClass *pSwapchain);
    VkRenderPass getRenderPassStruct() { return renderPass; }

    private:

    VkRenderPass renderPass;
    VkDevicesClass *device;
    VkSwapchainClass *swapchain;
};

#endif