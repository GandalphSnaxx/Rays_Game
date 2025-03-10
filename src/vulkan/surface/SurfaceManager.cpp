#include "SurfaceManager.hpp"
/**
 * @file vulkan/surface/SurfaceManager.cpp
 * @author Ray Richter
 * @brief SurfaceManager class definitions.
 */

/// @section Constructors

/// @brief Vulkan surface constructor. Requires access to VkInstance, GLFWwindow*, and VkSurfaceKHR
SurfaceManager::SurfaceManager(VulkanInstance *instance, WindowManager *window, const size_t &maxFIF) {
    DEBUG_MSG("Called SurfaceManager init constructor");
    VK_CHECK(init_(instance, window, maxFIF));
}

SurfaceManager::SurfaceManager() {
    DEBUG_MSG("Called SurfaceManager default constructor");
}

/// @brief Surface deconstructor
SurfaceManager::~SurfaceManager() {
    DEBUG_MSG("Called VkSurfaceClass deconstructor");
    vkDestroySurfaceKHR(pInstanceMgr_->getInstance(), surface_, nullptr);
}

/// @section Public Member Functions

VkResult SurfaceManager::init(VulkanInstance *instance, WindowManager *window, const size_t &maxFIF) {
    DEBUG_MSG("Initializing SurfaceManager with a function");
    return init_(instance, window, maxFIF);
}

/// @section Private Member Functions

VkResult SurfaceManager::init_(VulkanInstance *instance, WindowManager *window, const size_t &maxFIF) {
    DEBUG_MSG("\tInit SurfaceManager...");
    pInstanceMgr_ = instance;
    pWindowMgr_ = window;
    maxFIF_ = maxFIF;

    VkResult result = glfwCreateWindowSurface(pInstanceMgr_->getInstance(), pWindowMgr_->getWindow(), nullptr, &surface_);
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tInit done!");
    return result;
}
