#include "vk_instance.hpp"

/// @brief Instance constructor populates the VkInstance in the class
VkInstanceClass::VkInstanceClass() {
    instance = nullptr;
}

/// @brief Instance deconstructor
VkInstanceClass::~VkInstanceClass() {
    // Destroy the validation layers before the instance
    validationLayers->~VkValidationLayersClass();
    vkDestroyInstance(instance, nullptr);
}

/// @brief Initializes an instance contained in the class
/// @return VkResult
VkResult VkInstanceClass::init() {
    if (validationLayers->enabled() && !validationLayers->checkValidationLayerSupport()) {
        THROW_ERR("validation layers requested, but not available!");
    }

    // Configure application info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Configure instance creator info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = validationLayers->getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Add any validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (validationLayers->enabled()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers->size());
        createInfo.ppEnabledLayerNames = validationLayers->data();

        validationLayers->populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    // Create an instance
    VkResult result;
    result = vkCreateInstance(&createInfo, nullptr, &instance);

    // Add validation layers to the instance
    validationLayers->init(&instance);

    return result;
}

/// @brief A method for copying the private instance pointer
/// @return VkInstance
VkInstance VkInstanceClass::getInstance() {
    return instance;
}