#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Copilot reccommended section:

/// @brief A structure for initializing a window
/// @param width Initial window width
/// @param height Initial window height
/// @param title Initial window title, UTF-8 encoding
/// @param monitor Fullscreen monitor, or `NULL` for windowed
/// @param share A window to share resources with, or `NULL`
struct WindowInit {
    int width;
    int height;
    const char *title;
    GLFWmonitor *monitor = nullptr;
    GLFWwindow *share = nullptr;
};

/// @brief A class to handle window creation and management.
class WindowManager {
    public:
    WindowManager (const WindowInit &windowInit);
    WindowManager ();
    ~WindowManager();

    /// @brief Initalizes a GLFWwindow for Vulkan.
    /// @param windowInit Initial window parameters as `WindowInit`.
    /// @return `VkResult`
    VkResult    init        (const WindowInit &windowInit);
    /// @brief Initalizes a GLFWwindow for Vulkan.
    /// @param width Initial window width as `int`.
    /// @param height Initial window height as `int`.
    /// @param title Initial window title as UTF-8 `char*`
    /// @return `VkResult`
    VkResult    init        (const int width, const int height, const char* title);
    /// @brief Gets the current window.
    GLFWwindow* getWindow   () const { return window_; }
    /// @brief Sets frame buffer resized flag.
    void        resize      ()       { frameBufferResized_ = true; }
    void        notResize   ()       { frameBufferResized_ =false; }
    bool        resized     ()       { return frameBufferResized_; }
    
    private:
    bool        frameBufferResized_ = false; // Write only
    GLFWwindow* window_;                     // Read only

    VkResult    init_                       (const WindowInit &windowInit);
    static void framebufferResizeCallback_  (GLFWwindow* window, int width, int height);
};