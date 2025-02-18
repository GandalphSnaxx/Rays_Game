#include "vk_window.hpp"

/// @brief Initalize a GLFW window and its context for Vulkan to use
VkWindowClass::VkWindowClass() {
    window = nullptr;
    framebufferResized = false;
}

/// @brief VkWindowClass deconstructor
VkWindowClass::~VkWindowClass() {
    // Destroy a specific window and its context
    glfwDestroyWindow(window);
    // Terminate the GLFW library
    glfwTerminate();
}

/// @brief Initalize a GLFW window and its context.
/// @param windowParams A pointer to a structure containing information for window creation
/// @return VkResult - Vulkan error codes
VkResult VkWindowClass::init(const WindowInit *windowParams) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(
        windowParams->width, 
        windowParams->height, 
        windowParams->title, 
        windowParams->monitor, 
        windowParams->share);
    
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    // Return success
    return VK_SUCCESS;
}

/// @brief Get a pointer to the current window
/// @return A pointer to the window
GLFWwindow *VkWindowClass::getWindowP() {
    return window;
}