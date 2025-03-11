#pragma once
/**
 * @file vulkan/surface/SurfaceManager.hpp
 * @author Ray Richter
 * @brief SurfaceManager class definition.
 */
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vulkan/instance/VulkanInstance.hpp"
#include "vulkan/window/WindowManager.hpp"
#include "raysDebugHelper.hpp"

/// @brief VkSurfaceClass is a child class of VkOverheadClass. Contains functions related to a VkSurface
///     Requires access to _surface
class SurfaceManager {
public:
    SurfaceManager (VulkanInstance*, WindowManager*, const size_t&);
    SurfaceManager ();
    ~SurfaceManager();
    // Delete the default constructor to prevent its use
    // SurfaceManager::SurfaceManager() = delete;

    /// @brief Initalizes SurfaceManager for Vulkan.
    /// @param pInstanceMgr The instance to create a surface for.
    /// @param pWindowMgr The window to create a surface for.
    /// @param maxFIF Maximum number of frames in flight.
    /// @return `VkResult`
    VkResult init(VulkanInstance*, WindowManager*, const size_t&);
    VkResult cleanup();
    VkSurfaceKHR getSurface () const { return surface_;                     }
    VkInstance   getInstance() const { return pInstanceMgr_->getInstance(); }
    GLFWwindow*  getPWindow () const { return pWindowMgr_->getWindow();     }
    size_t       getMaxFIF  () const { return maxFIF_;                      }

private:
    VkSurfaceKHR    surface_;
    VulkanInstance* pInstanceMgr_;
    WindowManager*  pWindowMgr_;
    size_t          maxFIF_;

    VkResult init_(VulkanInstance*, WindowManager*, const size_t&);
};