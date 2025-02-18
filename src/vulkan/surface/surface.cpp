#include "surface.hpp"

/// @brief Vulkan surface constructor. Requires access to VkInstance, GLFWwindow*, and VkSurfaceKHR
VkSurfaceClass::VkSurfaceClass() {
    DEBUG_MSG("Called VkSurfaceClass constructor");
    
    // Create a surface
    VK_CHECK(glfwCreateWindowSurface(_instance, _window, nullptr, &_surface));
}

/// @brief Surface deconstructor
VkSurfaceClass::~VkSurfaceClass() {
    DEBUG_MSG("Called VkSurfaceClass deconstructor");
    
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
}