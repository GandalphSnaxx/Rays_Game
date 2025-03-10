#pragma once

#include "vulkan/instance/VulkanInstance.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define USE_64_BIT_MODE
#include <Validator.hpp>
#include <vector>

/// @brief A class to handle physical device selection and querying, and logical device creation and queue retrieval.
class DeviceManager {
    public:
    DeviceManager (VulkanInstance*);
    DeviceManager ();
    ~DeviceManager();

    /// @brief DeviceManager initalizer.
    /// @param instance The `VulkanInstance` to connect a device to.
    /// @return `VkResult`
    VkResult         init             (VulkanInstance*);
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice_; }
    VkDevice         getDevice        () const { return device_;         }
    VkQueue          getGraphicsQueue () const { return graphicsQueue_;  }
    VkQueue          getPresentQueue  () const { return presentQueue_;   }
    VkInstance       getInstance      () const { return pInstance_->getInstance(); }
    bool             validLayers      () const { return pInstance_->validLayers(); }

    // void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
    // void createLogicalDevice(
    //     VkPhysicalDevice physicalDevice, 
    //     VkSurfaceKHR surface, 
    //     const std::vector<const char*>& deviceExtensions, 
    //     bool enableValidationLayers, 
    //     const std::vector<const char*>& validationLayers);
    
    private:
    VkPhysicalDevice    physicalDevice_ = VK_NULL_HANDLE;
    VkDevice            device_;
    /// TODO: Move to CommandManager?
    VkQueue             graphicsQueue_;
    VkQueue             presentQueue_;
    VulkanInstance*     pInstance_;
    
    VkResult    init_               (VulkanInstance*);
    VkResult    pickPhysicalDevice_ ();
    VReturn_t   rateDevice_         (const VkPhysicalDevice&);
    /// TODO: Move to command manager init
    // VkResult createLogicalDevice_();
};