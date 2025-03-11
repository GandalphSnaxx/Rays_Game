#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#define ENGINE_NAME "Ray's Vulkan Engine"

/// @brief A class to handle Vulkan instance creation, validation layers, and debug messenger.
class VulkanInstance {
public:
    VulkanInstance (const std::vector<const char*> &validationLayers, const char *appName, const bool &enValLayers = false);
    VulkanInstance ();
    ~VulkanInstance();

    // Public functions

    VkResult    init        (const std::vector<const char*> &validationLayers, const char *appName, const bool &enValLayers = false);
    VkResult    cleanup     ();
    VkInstance  getInstance () const { return instance_; }
    bool        vLayersEn   () const { return enValidationLayers_; }
    std::vector<const char*> getVLayers() const { return validationLayers_; }
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData);
    
private:
    bool                        enValidationLayers_ = false; // Read only
    VkInstance                  instance_;                   // Read only
    VkDebugUtilsMessengerEXT    debugMessenger_;             // No read/write
    std::vector<const char*>    validationLayers_;           // Read only

    VkResult createInstance_(const std::vector<const char*> &validationLayers, const char *appName, const bool &enValLayers);
    VkResult setupValidationLayers_();
    // VkResult setupDebugMessenger_();
    bool checkValidationLayerSupport_(const std::vector<const char*> &validationLayers);
    std::vector<const char*> getRequiredExtensions_();

    void populateDebugMessengerCreateInfo_(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult CreateDebugUtilsMessengerEXT_(
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator);
    void DestroyDebugUtilsMessengerEXT_(const VkAllocationCallbacks* pAllocator);
};