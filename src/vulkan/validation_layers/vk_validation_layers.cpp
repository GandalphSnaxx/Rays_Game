#include "vk_validation_layers.hpp"

/// @brief Set any local variables to default values
VkValidationLayersClass::VkValidationLayersClass() {
    validationInstance = nullptr;
    debugMessenger = nullptr;
}

/// @brief Validation layer deconstructor
VkValidationLayersClass::~VkValidationLayersClass() {
    if (enabled()) {
        DestroyDebugUtilsMessengerEXT(*validationInstance, debugMessenger, nullptr);
    }
}

/// @brief Sets up any debug messengers in the provided instance
/// @param instance The instance to add an debug messengers to
/// @return VkResult
VkResult VkValidationLayersClass::init(VkInstance *instance) {
    // If validation layers are not enabled or no instance was given, no setup is required
    if (!enabled() || instance == VK_NULL_HANDLE) return;

    // Copy the instance to the local instance for the deconstructor
    validationInstance = instance;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    // Add debug messengers to the instance
    return CreateDebugUtilsMessengerEXT(*validationInstance, &createInfo, nullptr, &debugMessenger);
}

/// @brief Gets the list of validation layers from the class
/// @return The list of validation layers
std::vector<const char*> VkValidationLayersClass::getValidationLayers() {
    return validationLayers;
}

/// @brief Compares a list of availible layers to a list of layer properties we want
/// @return Return true if a layer is found
bool VkValidationLayersClass::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
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
/// @return The required list of extensions based on weather validation layers are enabled
std::vector<const char*> VkValidationLayersClass::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (VkValidationLayersClass::enabled()) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/// @brief Configure a structure to setup a debug messenger
/// @param createInfo The struct to be populated
void VkValidationLayersClass::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}