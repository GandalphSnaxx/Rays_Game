#include "vk_surface.hpp"

VkSurfaceClass::VkSurfaceClass() {}

VkSurfaceClass::~VkSurfaceClass() {
    vkDestroySurfaceKHR(instance->getInstance(), surface, nullptr);
}

/// @brief Initalizes a surface
/// @param instance The instance to initalize a surface in
/// @param window The window to initalize a surface in
/// @return VkResult
VkResult VkSurfaceClass::init(VkInstanceClass *pInstance, VkWindowClass *pWindow) {
    // Add a pointer to the instance and to the window into the class
    instance = pInstance;
    window = pWindow;
    // Create a surface
    return glfwCreateWindowSurface(instance->getInstance(), pWindow->getWindowP(), nullptr, &surface);
}