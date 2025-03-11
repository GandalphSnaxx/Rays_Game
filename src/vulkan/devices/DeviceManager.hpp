#pragma once

#include "vulkan/instance/VulkanInstance.hpp"
#include "vulkan/surface/SurfaceManager.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define USE_64_BIT_MODE
#include <Validator.hpp>
#include <vector>
#include <set>
#include "vulkan/swapchain/QueueFamilyIndices.hpp"
#include "DeviceExtensions.hpp"
#include "raysDebugHelper.hpp"

/// @brief A class to handle physical device selection and querying, and logical device creation and queue retrieval.
class DeviceManager {
    public:
    DeviceManager (VulkanInstance*, SurfaceManager*);
    DeviceManager ();
    ~DeviceManager();

    /// @brief DeviceManager initalizer.
    /// @param instance The `VulkanInstance` to connect a device to.
    /// @return `VkResult`
    VkResult         init             (VulkanInstance*, SurfaceManager*);
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice_;           }
    VkDevice         getDevice        () const { return device_;                   }
    VkQueue          getGraphicsQueue () const { return graphicsQueue_;            }
    VkQueue          getPresentQueue  () const { return presentQueue_;             }
    VkInstance       getInstance      () const { return pInstance_->getInstance(); }
    bool             vLayersEn        () const { return pInstance_->vLayersEn();   }
    std::vector<const char*>getVLayers() const { return pInstance_->getVLayers();  }
    VkSurfaceKHR     getSurface       () const { return pSurface_->getSurface();   }
    GLFWwindow*      getPWindow       () const { return pSurface_->getPWindow();   }
    size_t           getMaxFIF        () const { return pSurface_->getMaxFIF();    }
    
private:
    VkPhysicalDevice    physicalDevice_ = VK_NULL_HANDLE;
    VkDevice            device_;
    VkQueue             graphicsQueue_;
    VkQueue             presentQueue_;
    VulkanInstance*     pInstance_;
    SurfaceManager*     pSurface_;
    
    VkResult    init_               (VulkanInstance*, SurfaceManager*);
    VkResult    pickPhysicalDevice_ ();
    VReturn_t   rateDevice_         (const VkPhysicalDevice&);
    VkResult    createLogicalDevice_();
};