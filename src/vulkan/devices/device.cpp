#include "device.hpp"

/// @brief Device constructor and linker. Picks a suitable physical device that can support Vulkan. 
///     Must be called after VkSurfaceClass constructor
VkDeviceClass::VkDeviceClass() {
    DEBUG_MSG("Called VkDeviceClass constructor");
    VK_CHECK(_pickPhysicalDevice());
    VK_CHECK(_createLogicalDevice());
}

/// @brief Device deconstructor
VkDeviceClass::~VkDeviceClass() {
    DEBUG_MSG("Called VkDeviceClass deconstructor");
    vkDestroyDevice(_device, nullptr);
}

/// @brief Gets a gpu that can support Vulkan
/// @return VkResult
VkResult VkDeviceClass::_pickPhysicalDevice() {
    VkResult result;

    // Query the number of gpus
    uint32_t deviceCount = 0;
    ERROR_RETURN(vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr));

    // Check for error conditions
    if (deviceCount == 0) {
        // ERROR_RETURN(VK_ERROR_DEVICE_LOST);
        THROW_ERR("failed to find GPUs with Vulkan support!");
    }

    // Allocate an array to hold the physical device handles
    std::vector<VkPhysicalDevice> devices(deviceCount);
    ERROR_RETURN(vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data()));

    // Check if a physical device meets requirements
    for (const auto& device : devices) {
        if (_isDeviceSuitable(device)) {
            _physicalDevice = device;
            break;
        }
    }

    if (_physicalDevice == VK_NULL_HANDLE) {
        // ERROR_RETURN(VK_ERROR_DEVICE_LOST);
        THROW_ERR("failed to find a suitable GPU!");
    }
}

/// @brief Creates a logical device
/// @return VkResult
VkResult VkDeviceClass::_createLogicalDevice() {
    VkResult result;

    // Get the cmdqueue types supported by the physical device
    QueueFamilyIndices indices = _findQueueFamilies(_physicalDevice);

    // Create a list of cmdqueue creator structs
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // Fill the list
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = _deviceExtensions.data();

    if (_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(_validationLayers.size());
        createInfo.ppEnabledLayerNames = _validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    ERROR_RETURN(vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device));

    vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);

    return result;
}

/// @brief Checks if a suitable device meets out requirements
/// @param queriedDevice The device to be checked
/// @return True if the queried device has support for required extensions and the swapchain
bool VkDeviceClass::_isDeviceSuitable(VkPhysicalDevice queriedDevice) {
    // Get the command queues this device can support
    QueueFamilyIndices indices = _findQueueFamilies(queriedDevice);

    // Can this device support extensions at all?
    bool extensionsSupported = _checkDeviceExtensionSupport(queriedDevice);

    // Can this device support a swapchain?
    bool deviceSupportsSwapchain = false;
    if (extensionsSupported) {
        SwapchainSupportDetails swapChainSupport = _querySwapChainSupport(queriedDevice);
        deviceSupportsSwapchain = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && deviceSupportsSwapchain;
}

/// @brief Checks if a suitable device can support the required extensions
/// @param queriedDevice The device to be checked
/// @return True if the queried device can support every required extension
bool VkDeviceClass::_checkDeviceExtensionSupport(VkPhysicalDevice queriedDevice) {
    uint32_t extensionCount;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(queriedDevice, nullptr, &extensionCount, nullptr));

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(queriedDevice, nullptr, &extensionCount, availableExtensions.data()));

    std::set<std::string> requiredExtensions(_deviceExtensions.begin(), _deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

/// @brief Finds queue families for the physical device in the same class
/// @param queriedDevice The device to be checked
/// @return QueueFamilyIndices
VkDeviceClass::QueueFamilyIndices VkDeviceClass::_findQueueFamilies(VkPhysicalDevice queriedDevice) {
    // If no physical device has been created, do nothing
    if (_physicalDevice == nullptr) { return; }

    QueueFamilyIndices indices;

    // get the size queueFamilies should be
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(queriedDevice, &queueFamilyCount, nullptr);

    // Create and fill a list of queue family properties
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(queriedDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(queriedDevice, i, _surface, &presentSupport));

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
