#pragma once
/**
 * @file WindowManager.hpp
 * @brief A class to handle window creation and management.
 * @author Ray Richter
 * @note This file is part of the Ray's Engine project.
 */
#include "WindowInit.hpp"

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