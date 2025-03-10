#pragma once
/**
 * @brief Window initalizer struct for use with Vulkan.
 * @author Ray Richter
 */
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/// @brief A structure for initializing a window
/// @param width Initial window width
/// @param height Initial window height
/// @param title Initial window title, UTF-8 encoding
/// @param monitor Fullscreen monitor, or `NULL` for windowed
/// @param share A window to share resources with, or `NULL`
const struct WindowInit { public:
    constexpr WindowInit (
        const int &width, const int &height, const char *title, 
        GLFWmonitor *monitor, GLFWwindow *share) : 
        width(width), height(height), title(title), monitor(monitor), share(share) {}
        
    int width;
    int height;
    const char *title;
    GLFWmonitor *monitor = nullptr;
    GLFWwindow *share = nullptr;
};