#include "WindowManager.hpp"
#include "raysDebugHelper.hpp"

/// @brief Initalizes `GLFW` for Vulkan with a constructor.
/// @param `*window_init` A pointer to inital window parameters for window creation 
WindowManager::WindowManager(const WindowInit &windowInit) {
    DEBUG_MSG("Called WindowManager init constructor");
    VK_CHECK(init_(windowInit));
}

WindowManager::WindowManager() {
    DEBUG_MSG("Called WindowManager default constructor");
}

/// @brief Destroys any window related data
WindowManager::~WindowManager() {
    DEBUG_MSG("Called WindowManager deconstructor");
    // // Destroy the window and its context
    // glfwDestroyWindow(window_);
    // // Terminate the GLFW library
    // glfwTerminate();
}

/// @brief Initalizes a `GLFWwindow` for Vulkan
/// @param `*window_init` A pointer to inital window parameters for window creation:
///
/// - `int` Width
///
/// - `int` Height
///
/// - `const char*` UTF-8 encoded window title
///
/// - `*GLFWmonitor` Fullscreen monitor. Set to `NULL` for windowed mode
///
/// - `*GLFWwindow` A window to share resources with. Set to `NULL` for none
///
/// . @return VkResult
VkResult WindowManager::init(const WindowInit &windowInit) {
    DEBUG_MSG("Called WindowManager init function...");
    return init_(windowInit);
}

/// @brief Initalizes a `GLFWwindow` for Vulkan
/// @param width Initial window width as `int`
/// @param height Inital window height as `int`
/// @param title Inital window title as `UTF-8` encoded `const char*`
VkResult WindowManager::init(const int width, const int height, const char* title) {
    DEBUG_MSG("Called WindowManager init function...");
    if (width < 1 || height < 1 || title == "") { THROW_ERR("Initial window parameters out of bounds!"); }
    const WindowInit windowInit{width, height, title, nullptr, nullptr};
    return init_(windowInit);
}

VkResult WindowManager::cleanup() {
    // Destroy the window and its context
    glfwDestroyWindow(window_);
    // Terminate the GLFW library
    glfwTerminate();
    return VK_SUCCESS;
}

/// @brief Internal window initializer
/// @param windowInit Inital window parameters
/// @return `VkResult`
VkResult WindowManager::init_(const WindowInit &windowInit) {
    DEBUG_MSG("\tInitalizing a window...");
    
    frameBufferResized_ = false;

    // Initialize the GLFW library
    glfwInit();

    // Set the specified window hint to the desired value
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create a window with the initial window parameters
    window_ = glfwCreateWindow(
        windowInit.width, 
        windowInit.height, 
        windowInit.title, 
        windowInit.monitor, 
        windowInit.share);
    
    // Link the window to this class' window and link the callback function
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback_);

    DEBUG_MSG("\tDone!");
    return VK_SUCCESS;
}

/// @brief The function called when a window is resized
/// @param window The `GLFWwindow` that has been resized
/// @param width The new `width`
/// @param height The new `height`
void WindowManager::framebufferResizeCallback_(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    app->resize();
}