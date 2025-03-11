#include "SwapchainManager.hpp"

/// @section Constructors and Deconstructor

SwapchainManager::SwapchainManager(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("Called SwapchainManager init constructor");
    VK_CHECK(init_(pDeviceMgr));
}

SwapchainManager::SwapchainManager() {
    DEBUG_MSG("Called SwapchainManager default constructor");
}

SwapchainManager::~SwapchainManager() {
    DEBUG_MSG("Called SwapchainManager deconstructor");
    cleanupSwapchain_();
    // vkDestroyCommandPool(device_, commandPool_, nullptr);
}

/// @section Public Member Functions

VkResult SwapchainManager::init(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("Called SwapchainManager init function");
    return init_(pDeviceMgr);
}

VkResult SwapchainManager::remake() {
    // Handle minimization
    int width = 0, height = 0;
    glfwGetFramebufferSize(pDeviceMgr_->getPWindow(), &width, &height);
    // Idle while the window is minimized
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(pDeviceMgr_->getPWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(pDeviceMgr_->getDevice());

    cleanupSwapchain_();

    init_(pDeviceMgr_);
    // createImageViews();
    // createFramebuffers();
    return VK_SUCCESS;
}

/// @section Private Member Functions

VkResult SwapchainManager::init_(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("\tInitalizing SwapchainManager...");
    pDeviceMgr_ = pDeviceMgr;
    VkResult result;

    result = createSwapchain_();
    if (result != VK_SUCCESS) return result;
    result = createImageViews_();
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tDone!");
    return result;
}

void SwapchainManager::cleanupSwapchain_() {
    if (getDevice() == nullptr) THROW_ERR("SwapchainManager ERROR: Swapchain init failed! Can't deconstruct!");

    // Destroy image views
    for (size_t i = 0; i < swapchainImageViews_.size(); i++) {
        DEBUG_MSG("\tDestroying image view " << i);
        vkDestroyImageView(getDevice(), swapchainImageViews_[i], nullptr);
    }

    // Destroy swapchain
    DEBUG_MSG("\tDestroying swapchain");
    vkDestroySwapchainKHR(getDevice(), swapchain_, nullptr);
}

VkSurfaceFormatKHR SwapchainManager::chooseSwapSurfaceFormat_(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    // Search for a format in the list that meets our requirements
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapchainManager::chooseSwapPresentMode_(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapchainManager::chooseSwapExtent_(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width  = std::clamp(actualExtent.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width );
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VkResult SwapchainManager::createSwapchain_() {
    VkResult result;
    DEBUG_MSG("\t\tCreating swapchain...");
    SupportDetails_ swapchainSupport{getPhysicalDevice(), getSurface()};
    
    VkSurfaceFormatKHR  surfaceFormat   = chooseSwapSurfaceFormat_  (swapchainSupport.formats);
    VkPresentModeKHR    presentMode     = chooseSwapPresentMode_    (swapchainSupport.presentModes);
    VkExtent2D          extent          = chooseSwapExtent_         (swapchainSupport.capabilities, getPWindow());

    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface          = getSurface();
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    result = queueFamilyIndices_.find(getPhysicalDevice(), getSurface());
    if (result != VK_SUCCESS) return result;

    uint32_t queueFamilyIndices[] = {queueFamilyIndices_.graphicsFamily.value(), queueFamilyIndices_.presentFamily.value()};

    if (queueFamilyIndices_.graphicsFamily != queueFamilyIndices_.presentFamily) {
        createInfo.imageSharingMode         = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount    = 2;
        createInfo.pQueueFamilyIndices      = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount    = 0; // Optional
        createInfo.pQueueFamilyIndices      = nullptr; // Optional
    }

    createInfo.preTransform     = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode      = presentMode;
    createInfo.clipped          = VK_TRUE;
    createInfo.oldSwapchain     = VK_NULL_HANDLE;

    result = vkCreateSwapchainKHR(getDevice(), &createInfo, nullptr, &swapchain_);
    if (result != VK_SUCCESS) return result;

    result = vkGetSwapchainImagesKHR(getDevice(), swapchain_, &imageCount, nullptr);
    if (result != VK_SUCCESS) return result;

    swapchainImages_.resize(imageCount);

    result = vkGetSwapchainImagesKHR(getDevice(), swapchain_, &imageCount, swapchainImages_.data());
    if (result != VK_SUCCESS) return result;

    swapchainImageFormat_   = surfaceFormat.format;
    swapchainExtent_        = extent;

    DEBUG_MSG("\t\tSwapchain created!");
    return result;
}

VkResult SwapchainManager::createImageViews_() {
    VkResult result;
    DEBUG_MSG("\t\tCreating swapchain image views...");

    // Resize the list to fit all the images we'll be creating
    swapchainImageViews_.resize(swapchainImages_.size());

    // Iterate through the swapchain images
    for (size_t i = 0; i < swapchainImages_.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        // Fill out the information struct
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages_[i];
        // Can treat images as 1D, 2D, 3D, and Cube maps
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchainImageFormat_;
        // Setup color mapping. We are using default settings
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        // Describe the image's purpose and which part should be accessed
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        // Create the image view
        result = vkCreateImageView(getDevice(), &createInfo, nullptr, &swapchainImageViews_[i]);
        if (result != VK_SUCCESS) return result;

    }
    DEBUG_MSG("\t\tSwapchain image views created!");
    return result;
}

/// @section Member Struct Function Definitions

VkResult SwapchainManager::SupportDetails_::query(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface) {
    VkResult result;

    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
    if (result != VK_SUCCESS) return result;

    uint32_t formatCount;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (result != VK_SUCCESS) return result;

    if (formatCount != 0) {
        formats.resize(formatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
        if (result != VK_SUCCESS) return result;
    }

    uint32_t presentModeCount;
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (result != VK_SUCCESS) return result;

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
        if (result != VK_SUCCESS) return result;
    }
    return result;
}