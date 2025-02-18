#ifndef VULKAN_INSTANCE_HPP__
#define VULKAN_INSTANCE_HPP__

#include "overhead.hpp"
#include <vector>

#define ENGINE_NAME "Ray's Vulkan Engine"
#ifdef DEBUG
#define EN_VAL_LAYERS true
#else
#define EN_VAL_LAYERS false
#endif

/// @brief A class containing variables and functions related to an instance before instance creation.
class VkInstanceOverheadClass : virtual public VkOverheadClass {
    protected:
    // If DEBUG is defined, enable validation layers
    const bool _enableValidationLayers = EN_VAL_LAYERS;

    // A list of validation layers
    const std::vector<const char*> _validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
};

/// @brief A class containing variables and functions related to a Vulkan Instance. Child class of VkInstanceOverheadClass, grandchild class of VkOverheadClass.
///     Requires access to _instance, _enableValidationLayers
class VkInstanceClass : virtual public VkInstanceOverheadClass {
    public:
    
    VkInstanceClass(const char *appName);
    ~VkInstanceClass();
    // Delete the default constructor to prevent its use
    VkInstanceClass::VkInstanceClass() = delete;

    protected:

    private:

    // Private variables
    VkDebugUtilsMessengerEXT _debugMessenger;

    // Private functions
    bool _checkValidationLayerSupport();
    std::vector<const char*> _getRequiredExtensions();
    void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult _setupValidationLayers();

    // Debug utils functions
    VkResult _CreateDebugUtilsMessengerEXT(
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator);
    void _DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
    static VKAPI_ATTR VkBool32 VKAPI_CALL VkInstanceClass::_debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData);
};

#endif