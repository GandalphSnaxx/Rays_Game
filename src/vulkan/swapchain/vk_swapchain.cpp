#include "vk_swapchain.hpp"

VkSwapchainClass::VkSwapchainClass() {}

VkSwapchainClass::~VkSwapchainClass() {
    cleanupSwapchain();
}

/// @brief Initalize a swapchain to a device
/// @param pDevice A pointer to the device's class
/// @return VkResult
VkResult VkSwapchainClass::init(VkDevicesClass *pDevice) {
    device = pDevice;
    return createSwapchain();
}

/// @brief Search for a format in a list that meets our requirements
/// @param availableFormats The list of formats to be searched
/// @return The format that meets our requirements
VkSurfaceFormatKHR VkSwapchainClass::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    // Search for a format in the list that meets our requirements
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

/// @brief Search for a present mode in a list that meets our requirements
/// @param availableFormats The list of present modes to be searched
/// @return The present mode that meets our requirements
VkPresentModeKHR VkSwapchainClass::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

/// @brief Get the extent we want based on capabilities
/// @param capabilities 
/// @return VkExtent2D
VkExtent2D VkSwapchainClass::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(device->getPSurface()->getPWindow()->getWindowP(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

/// @brief Configure image views for the swapchain
/// @return VkResult
VkResult VkSwapchainClass::createImageViews() {
    // Resize the list to fit all the images we'll be creating
    swapChainImageViews.resize(swapChainImages.size());

    // Iterate through the swapchain images
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        // Fill out the information struct
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        // Can treat images as 1D, 2D, 3D, and Cube maps
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
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
        return vkCreateImageView(device->getDevice(), &createInfo, nullptr, &swapChainImageViews[i]);

    }
}

/// @brief Destroy an old swapchain
void VkSwapchainClass::cleanupSwapchain() {
    // Destroy buffers
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(device->getDevice(), swapChainFramebuffers[i], nullptr);
    }

    // Destroy image views
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(device->getDevice(), swapChainImageViews[i], nullptr);
    }

    // Destroy swapchain
    vkDestroySwapchainKHR(device->getDevice(), swapChain, nullptr);
}

/// @brief Create a swapchain linked to the local device
/// @return VkResult
VkResult VkSwapchainClass::createSwapchain() {
    VkResult result;

    // Check for swapchain support details in the current device
    SwapChainSupportDetails swapChainSupport = device->swapchainSupport();

    // Set format, present mode, and extent
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    // Get the number of images to be in the swapchain
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // Swapchain creator struct
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface->getSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Set indicies
    QueueFamilyIndices indices = device->queueFamilies();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Create a swapchain
    ERROR_RETURN(vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapChain));

    // Map the swapchain images in memory
    vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device->getDevice(), swapChain, &imageCount, swapChainImages.data());

    // Set the format and extent
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    ERROR_RETURN(createImageViews());

    return result;
}

/// @brief Rebuild the swapchain if the window size changes
VkResult VkSwapchainClass::recreateSwapchain(VkRenderPassClass *renderPass) {
    VkResult result;

    // Handle minimization
    int width = 0, height = 0;
    glfwGetFramebufferSize(window->getWindowP(), &width, &height);
    // Idle while the window is minimized
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window->getWindowP(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device->getDevice());

    cleanupSwapchain();

    ERROR_RETURN(createSwapchain());
    ERROR_RETURN(createImageViews());
    ERROR_RETURN(createFramebuffers(renderPass));

    return result;
}

/// @brief Create frame buffers
VkResult VkSwapchainClass::createFramebuffers(VkRenderPassClass *renderPass) {
    VkResult result;

    // Resize the container to hold all frame buffers
    swapChainFramebuffers.resize(swapChainImageViews.size());

    // Iterate through image views and create frame buffers for them
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };
    
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->getRenderPassStruct();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;
    
        ERROR_RETURN(vkCreateFramebuffer(device->getDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]));
    }

    return result;
}