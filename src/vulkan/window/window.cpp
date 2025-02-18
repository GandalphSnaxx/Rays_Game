#include "window.hpp"

/// @brief Constructs a GLFW window for Vulkan
/// @param window_init Initial window parameters (ie. width, height, title)
VkWindowClass::VkWindowClass(const WindowInit *window_init) {
    DEBUG_MSG("Called VkWindowClass constructor");
    
    _framebufferResized = false;

    // Initialize the GLFW library
    glfwInit();

    // Set the specified window hint to the desired value
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create a window with the initial window parameters
    _window = glfwCreateWindow(
        window_init->width, 
        window_init->height, 
        window_init->title, 
        window_init->monitor, 
        window_init->share);
    
    // Link the window to this class' window and link the callback function
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, _framebufferResizeCallback);
}

/// @brief Vulkan Window Class deconstructor. Terminates GLFW
VkWindowClass::~VkWindowClass() {
    DEBUG_MSG("Called VkWindowClass deconstructor");
    
    // Destroy the window and its context
    glfwDestroyWindow(_window);
    // Terminate the GLFW library
    glfwTerminate();
}

/// @brief The function called when a window is resized
/// @param window The window that has been resized
/// @param width The new width
/// @param height The new height
void VkWindowClass::_framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<VkWindowClass*>(glfwGetWindowUserPointer(window));
    app->_framebufferResized = true;
}