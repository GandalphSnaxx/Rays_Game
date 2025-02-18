#ifndef VK_SURFACE_HPP__
#define VK_SURFACE_HPP__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/instance/vk_instance.hpp"
#include "vulkan/swapchain/vk_swapchain.hpp"
#include "vulkan/window/vk_window.hpp"
#include "raysDebugHelper.hpp"

class VkSurfaceClass {
    public:

    // Constructor and deconstructor
    VkSurfaceClass();
    ~VkSurfaceClass();

    VkResult init(VkInstanceClass *pInstance, VkWindowClass *pWindow);
    VkSurfaceKHR getSurface() { return surface; }
    VkInstanceClass *getPInstance() { return instance; }
    VkWindowClass *getPWindow() { return window; }

    private:

    VkSurfaceKHR surface;
    VkInstanceClass *instance;
    VkWindowClass *window;
};

#endif