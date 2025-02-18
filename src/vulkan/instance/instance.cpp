#include "instance.hpp"

/// @brief Constructs a Vulkan instance for this application
/// @param appName Name of the application
VkInstanceClass::VkInstanceClass(const char *appName) {
    DEBUG_MSG("Called VkInstanceClass constructor");
    
    if (_enableValidationLayers && !_checkValidationLayerSupport()) {
        THROW_ERR("validation layers requested, but not available!");
    }

    // Configure application info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = ENGINE_NAME;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Configure instance creator info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = _getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Add any validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
        createInfo.ppEnabledLayerNames = _validationLayers.data();

        _populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    // Create an instance
    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &_instance));

    // Add validation layers to the instance
    VK_CHECK(_setupValidationLayers());
}

/// @brief Instance deconstructor
VkInstanceClass::~VkInstanceClass() {
    DEBUG_MSG("Called VkInstanceClass deconstructor");
    
    if (_enableValidationLayers) {
        _DestroyDebugUtilsMessengerEXT(nullptr);
    }
    vkDestroyInstance(_instance, nullptr);
}

/// @brief Compares a list of availible layers to a list of layer properties we want
/// @return Return true if a layer is found
bool VkInstanceClass::_checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : _validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

/// @brief A method for retrieving a list of required extensions
/// @return The required list of extensions. If validation layers are enabled, include them
std::vector<const char*> VkInstanceClass::_getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (_enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/// @brief Configure a structure to setup a debug messenger
/// @param createInfo The struct to be populated
void VkInstanceClass::_populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = _debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

/// @brief Adds any validation layers to this instance
/// @return VkResult
VkResult VkInstanceClass::_setupValidationLayers() {
    // If validation layers are not enabled, no setup is required
    if (!_enableValidationLayers) { return VK_SUCCESS; }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    _populateDebugMessengerCreateInfo(createInfo);

    // Add debug messengers to the instance
    return _CreateDebugUtilsMessengerEXT(&createInfo, nullptr);
}

/* *************************************************** *
 * Debug utilities for my vulkan engine implementation *
 * *************************************************** */

/// @brief Debug callback
/// @param pCreateInfo Setup struct
/// @param pAllocator 
/// @return Vulkan result code
VkResult VkInstanceClass::_CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(_instance, pCreateInfo, pAllocator, &_debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/// @brief Free any memory used by the debug utils
/// @param pAllocator 
void VkInstanceClass::_DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(_instance, _debugMessenger, pAllocator);
    }
}

/// @brief A function for printing validation errors
/// @param messageSeverity Message severity
/// @param messageType Message type
/// @param pCallbackData 
/// @param pUserData 
/// @return VkBool32 - false
VKAPI_ATTR VkBool32 VKAPI_CALL VkInstanceClass::_debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData) {

    CERR("Validation Layer Error: ", pCallbackData->pMessage);

    return VK_FALSE;
}