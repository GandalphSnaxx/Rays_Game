#include "DeviceManager.hpp"
#include "properties/DeviceProperties_constants.hpp"
#include "raysDebugHelper.hpp"
#include <map>

/**
 * @section Public Functions
 */

DeviceManager::DeviceManager(VulkanInstance *instance, SurfaceManager *surface) {
    DEBUG_MSG("Initalizing DeviceManager with a constructor...");
    VK_CHECK(init_(instance, surface));
}

DeviceManager::DeviceManager() {
    DEBUG_MSG("Called DeviceManager default constructor");
}

/// @brief DeviceManager deconstructor
DeviceManager::~DeviceManager() {
    DEBUG_MSG("Deconstructing DeviceManager...");
}


VkResult DeviceManager::init(VulkanInstance *instance, SurfaceManager *surface) {
    DEBUG_MSG("Initalizing DeviceManager with a function...");
    return init_(instance, surface);
}

/**
 * @section Private Functions
 */

VkResult DeviceManager::init_(VulkanInstance *instance, SurfaceManager *surface) {
    DEBUG_MSG("\tInitalizing DeviceManager...");
    pInstance_ = instance;
    pSurface_ = surface;

    // DEBUG_MSG("\tPicking a physical device...");
    VkResult result = pickPhysicalDevice_();
    if (result != VK_SUCCESS) return result;

    // DEBUG_MSG("\tCreating a logical device...");
    result = createLogicalDevice_();
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tInit done!");
    return result;
}

/**
 * @subsection Physical Device Functions
 */


VkResult DeviceManager::pickPhysicalDevice_() {
    DEBUG_MSG("\tPicking a physical device...");

    VkResult result = VK_SUCCESS;

    // Query the number of gpus
    uint32_t deviceCount = 0;
    ERROR_RETURN(vkEnumeratePhysicalDevices(pInstance_->getInstance(), &deviceCount, nullptr));

    // Check for error conditions
    if (deviceCount == 0) {
        // ERROR_RETURN(VK_ERROR_DEVICE_LOST);
        THROW_ERR("DeviceManager ERROR: Failed to find any GPU with Vulkan support!");
    }

    // Allocate a list to hold the physical device handles
    std::vector<VkPhysicalDevice> devices(deviceCount);
    ERROR_RETURN(vkEnumeratePhysicalDevices(pInstance_->getInstance(), &deviceCount, devices.data()));

    // Get device ratings
    VReturn_t bestScore = 0;
    for (const auto &device : devices) {
        VReturn_t thisScore = rateDevice_(device);
        DEBUG_MSG("\t\tDevice found with score: " << thisScore);
        if (thisScore > bestScore) {
            bestScore = thisScore;
            physicalDevice_ = device;
        }
    }

    if (physicalDevice_ == VK_NULL_HANDLE) {
        // ERROR_RETURN(VK_ERROR_DEVICE_LOST);
        THROW_ERR("DeviceManager ERROR: Failed to find a suitable GPU!");
    }
    DEBUG_MSG("\t\tBest score: " << bestScore);
    DEBUG_MSG("\tPhysical device picked!");
    return result;
}

/// @brief Checks for extension support on a queried `VkPhysicalDevice`
/// @param queriedDevice The `VkPhysicalDevice` to be rated
/// @return `VReturn_t` - Validator return value
VReturn_t DeviceManager::rateDevice_(const VkPhysicalDevice &queriedDevice) {
    VReturn_t deviceScore = 0;
    const VkPhysicalDevicePropertiesValidator   validProperties;
    const VkPhysicalDeviceFeaturesValidator     validFeatures;

    // Get the queried device's features and properties
    VkPhysicalDeviceProperties                  deviceProperties;
    VkPhysicalDeviceFeatures                    deviceFeatures;
    vkGetPhysicalDeviceProperties              (queriedDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures                (queriedDevice, &deviceFeatures  );

    deviceScore += validProperties             (deviceProperties);
    deviceScore += validFeatures               (deviceFeatures);

    return deviceScore;
}

VkResult DeviceManager::createLogicalDevice_() {
    VkResult result;
    DEBUG_MSG("\tCreating a logical device...");
    QueueFamilyIndices indices(physicalDevice_, pSurface_->getSurface());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

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

    createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    if (pInstance_->vLayersEn()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(pInstance_->getVLayers().size());
        createInfo.ppEnabledLayerNames = pInstance_->getVLayers().data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    result = vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_);
    if (result != VK_SUCCESS) return result;

    vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily.value(),  0, &presentQueue_ );

    DEBUG_MSG("\tLogical device created!");
    return result;
}