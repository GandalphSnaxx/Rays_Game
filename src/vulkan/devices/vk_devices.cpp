#include "vk_devices.hpp"

VkDevicesClass::VkDevicesClass() {}

VkDevicesClass::~VkDevicesClass() {
    vkDestroyDevice(device, nullptr);
}

VkResult VkDevicesClass::init(VkSurfaceClass *pSurface) {
    instance = pSurface->getPInstance();
    surface = pSurface;

    VkResult result;

    // Pick physical device
    ERROR_RETURN(pickPhysicalDevice());
    // Create logical device
    ERROR_RETURN(createLogicalDevice());

    return result;
}

/// @brief Picks the first physical device that meets every extension requirement
/// @return 
VkResult VkDevicesClass::pickPhysicalDevice() {
    VkResult result;
    // Query the number of gpus
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, nullptr);
    if (result != VK_SUCCESS) { return result; }

    // No GPU found with Vulkan support!
    if (deviceCount == 0) { return VK_ERROR_DEVICE_LOST; }

    // Allocate an array to hold the physical device handles
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, physicalDevices.data());
    if (result != VK_SUCCESS) { return result; }
    
    // Check if a physical device meets requirements
    for (const auto& tempDevice : physicalDevices) {
        if (isDeviceSuitable(tempDevice)) {
            physicalDevice = tempDevice;
            break;
        }
    }

    // Failed to find a suitable GPU!
    if (physicalDevice == VK_NULL_HANDLE) { return VK_ERROR_DEVICE_LOST; }
    
    return result;
}

/// @brief Creates a logical device
/// @return VkResult
VkResult VkDevicesClass::createLogicalDevice() {
    VkResult result;

    // Get the queue types supported by the physical device
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    // Create a list of queue creator structs
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

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (instance->getValidationLayersEnabled()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(instance->getValidationLayersSize());
        createInfo.ppEnabledLayerNames = instance->getValidationLayersData();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

    return result;
}

/// @brief Checks if a suitable device meets out requirements
/// @param suitableDevice The device to be checked
/// @return 
bool VkDevicesClass::isDeviceSuitable(VkPhysicalDevice suitableDevice) {
    QueueFamilyIndices indices = findQueueFamilies(suitableDevice);

    bool extensionsSupported = checkDeviceExtensionSupport(suitableDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(suitableDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

/// @brief Checks if a suitable device can support the required extensions
/// @param suitableDevice The device to be checked
/// @return 
bool VkDevicesClass::checkDeviceExtensionSupport(VkPhysicalDevice suitableDevice) {
    uint32_t extensionCount;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(suitableDevice, nullptr, &extensionCount, nullptr));

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(suitableDevice, nullptr, &extensionCount, availableExtensions.data()));

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

/// @brief Checks if a suitable device has support for a swapchain
/// @param suitableDevice The device to be checked
/// @return Swapchain details
SwapChainSupportDetails VkDevicesClass::querySwapChainSupport(VkPhysicalDevice suitableDevice) {
    SwapChainSupportDetails details;

    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(suitableDevice, surface->getSurface(), &details.capabilities));

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(suitableDevice, surface->getSurface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(suitableDevice, surface->getSurface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(suitableDevice, surface->getSurface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(suitableDevice, surface->getSurface(), &presentModeCount, details.presentModes.data());
    }

    return details;
}

/// @brief Finds queue families for the physical device in the same class
/// @return 
QueueFamilyIndices VkDevicesClass::findQueueFamilies(VkPhysicalDevice suitableDevice) {
    // If no physical device has been created, do nothing
    if (physicalDevice == nullptr) { return; }

    QueueFamilyIndices indices;

    // get the size queueFamilies should be
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(suitableDevice, &queueFamilyCount, nullptr);

    // Create and fill a list of queue family properties
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(suitableDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(suitableDevice, i, surface->getSurface(), &presentSupport));

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