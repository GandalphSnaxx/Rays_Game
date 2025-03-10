#include "VulkanInstance.hpp"
#include "raysDebugHelper.hpp"

/**
 * @section VulkanInstance class
 */

/// @brief Initalizes a Vulkan instance at construction
/// @param validationLayers A list of validation layers to enable as `std::vector<const char*>`. 
/// If the list is empty, validation layers are disabled
/// @param appName Name of the application as `const char*`
VulkanInstance::VulkanInstance(const std::vector<const char*> &validationLayers, const char *appName) {
    DEBUG_MSG("Initalizing VulkanInstance with a constructor...");
    VK_CHECK(createInstance_(validationLayers, appName));
    VK_CHECK(setupValidationLayers_());
    // VK_CHECK(setupDebugMessenger_());
}

VulkanInstance::VulkanInstance() {
    DEBUG_MSG("Called VulkanInstance default constructor");
}

/// @brief Cleans up this Vulkan instance
VulkanInstance::~VulkanInstance() {
    DEBUG_MSG("Deconstructing VulkanInstance...");
    if (enValidationLayers_) { DestroyDebugUtilsMessengerEXT_(nullptr); }
    vkDestroyInstance(instance_, nullptr);
}

/// @brief Initalizes a Vulkan instance
/// @param validationLayers A list of validation layers to enable as `std::vector<const char*>`. 
/// If the list is empty, validation layers are disabled
/// @param appName Name of the application as `const char*`
/// @return `VkResult`
VkResult VulkanInstance::init(const std::vector<const char*> &validationLayers, const char *appName) {
    DEBUG_MSG("Initalizing VulkanInstance with a function...");
    VkResult result;
    ERROR_RETURN(createInstance_(validationLayers, appName));
    ERROR_RETURN(setupValidationLayers_());
    // ERROR_RETURN(setupDebugMessenger_());
    return result;
}

/**
 * @section Instance
 */

/// @brief Creates a Vulkan instance
/// @param validationLayers A list of `const char*`s
/// @param appName Application name as `const char*`
/// @return `VkResult`
VkResult VulkanInstance::createInstance_(const std::vector<const char*> &validationLayers, const char *appName) {
    DEBUG_MSG("\tCreating a Vulkan Instance");
    VkResult result;
    // Get the number of validation layers to enable
    const uint32_t numValidationLayers = static_cast<uint32_t>(validationLayers.size());
    if (numValidationLayers != 0) { 
        DEBUG_MSG("\t\tValidation Layers Enabled");
        enValidationLayers_ = true;
    } else {
        DEBUG_MSG("\t\tValidation Layers Disabled");
        enValidationLayers_ = false;
    }

    // Configure application info
    /// TODO: Configure app version
    VkApplicationInfo appInfo{};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = appName;
    appInfo.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName         = ENGINE_NAME;
    appInfo.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    // Configure instance creator info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    // Add required extensions to the instance
    auto extensions = getRequiredExtensions_();
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames  = extensions.data();

    // Add any validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    createInfo.enabledLayerCount = numValidationLayers;
    if (enValidationLayers_) {
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo_(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.pNext = nullptr;
    }

    // Initalize the instance
    return vkCreateInstance(&createInfo, nullptr, &instance_);
}

/**
 * @section Validation layers
 */

/// @brief Adds any validation layers to this instance
/// @return `VkResult`
VkResult VulkanInstance::setupValidationLayers_() {
    // If validation layers are not enabled, no setup is required
    if (!enValidationLayers_) { return VK_SUCCESS; }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo_(createInfo);

    // Add debug messengers to the instance
    return CreateDebugUtilsMessengerEXT_(&createInfo, nullptr);
}

/// @brief Compares a list of availible layers to a list of layer properties we want
/// @return Return `true` if validation layers are supported
bool VulkanInstance::checkValidationLayerSupport_(const std::vector<const char*> &validationLayers) {
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

/**
 * @section Extensions
 */

/// @brief A method for retrieving a list of required extensions
/// @return The required list of extensions. If validation layers are enabled, include them
std::vector<const char*> VulkanInstance::getRequiredExtensions_() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Add validation layers extension if enabled
    if (enValidationLayers_) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/**
 * @section Debug utilities
 */

/// @brief Debug callback
/// @param pCreateInfo Setup struct
/// @param pAllocator A pointer to allocation callbacks as `VkAllocationCallbacks*`
/// @return `VkResult`
VkResult VulkanInstance::CreateDebugUtilsMessengerEXT_(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance_, pCreateInfo, pAllocator, &debugMessenger_);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/// @brief Configure a structure to setup a debug messenger
/// @param createInfo The struct to be populated
void VulkanInstance::populateDebugMessengerCreateInfo_(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = // Bytefield
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =    // Bytefield
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

/// @brief Free any memory used by the debug utils
/// @param pAllocator A pointer to allocation callbacks as `VkAllocationCallbacks*`
void VulkanInstance::DestroyDebugUtilsMessengerEXT_(const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance_, debugMessenger_, pAllocator);
    }
}

/// @brief A function for printing validation errors
/// @param messageSeverity Message severity
/// @param messageType Message type
/// @param pCallbackData 
/// @param pUserData 
/// @return `VkBool32` false
VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData) {

    CERR("Validation Layer Error: ", pCallbackData->pMessage);

    return VK_FALSE;
}