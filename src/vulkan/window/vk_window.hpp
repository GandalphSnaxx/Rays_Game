#ifndef VK_WINDOW_HPP__
#define VK_WINDOW_HPP__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/vk_handler.hpp"
#include "raysDebugHelper.hpp"

/// @brief A structure for initializing a window
/// @param width Initial window width
/// @param height Initial window height
/// @param title Initial window title, UTF-8 encoding
/// @param monitor Fullscreen monitor, or NULL for windowed
/// @param share A window to share resources with, or NULL
struct WindowInit {
    int width;
    int height;
    const char *title;
    GLFWmonitor *monitor = nullptr;
    GLFWwindow *share = nullptr;
};

class VkWindowClass {
    public:

    // Constructor, default constructor, and deconstructor
    VkWindowClass();
    ~VkWindowClass();

    VkResult init(const WindowInit *windowParams);
    GLFWwindow *getWindowP();

    private:

    // Private member variables
    GLFWwindow *window;
    bool framebufferResized = false;

    // Private member functions

    /// @brief The function called when a window is resized
    /// @param window The window that has been resized
    /// @param width The new width
    /// @param height The new height
    static void VkWindowClass::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto app = reinterpret_cast<VkWindowClass*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
};

#endif