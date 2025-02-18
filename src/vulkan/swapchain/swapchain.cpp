#include "swapchain.hpp"

/// @brief Swapchain constructor
VkSwapchainClass::VkSwapchainClass() {
    DEBUG_MSG("Called VkSwapchainClass constructor");
    VK_CHECK(_createSwapchain());
    VK_CHECK(_createImageViews());
    VK_CHECK(_createRenderPass());
}

/// @brief Swapchain deconstructor
VkSwapchainClass::~VkSwapchainClass() {
    DEBUG_MSG("Called VkSwapchainClass deconstructor");
    _cleanupSwapchain();
    _cleanupRenderPass();
}

/// @brief Finds a format that matches predefined requirements
/// @param availableFormats A list of formats to be searched
/// @return The format that meets the requirements
VkSurfaceFormatKHR VkSwapchainClass::_chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

/// @brief Finds a present mode that meets the predefined requirements
/// @param availablePresentModes A list of present modes to be searched
/// @return VkPresentModeKHR that matches the requirements
VkPresentModeKHR VkSwapchainClass::_chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

/// @brief Gets the extent we want for our swapchain
/// @param capabilities The current swapchain capabilities
/// @return VkExtent2D
VkExtent2D VkSwapchainClass::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

/// @brief Creates a frame buffer for every image view
/// @return VkResult
VkResult VkSwapchainClass::_createFramebuffers() {
    VkResult result;
    // Resize the container to hold all frame buffers
    _swapchainFramebuffers.resize(_swapchainImageViews.size());

    // Iterate through image views and create frame buffers for them
    for (size_t i = 0; i < _swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
            _swapchainImageViews[i]
        };
    
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = _swapchainExtent.width;
        framebufferInfo.height = _swapchainExtent.height;
        framebufferInfo.layers = 1;
    
        ERROR_RETURN(vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapchainFramebuffers[i]));
    }

    return result;
}

/// @brief Creates image views for every swapchain image
/// @return VkResult
VkResult VkSwapchainClass::_createImageViews() {
    DEBUG_MSG("\tCreating image views...");
    VkResult result;

    // Resize the list to fit all the images we'll be creating
    _swapchainImageViews.resize(_swapchainImages.size());

    // Iterate through the swapchain images
    for (size_t i = 0; i < _swapchainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        // Fill out the information struct
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapchainImages[i];
        // Can treat images as 1D, 2D, 3D, and Cube maps
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapchainImageFormat;
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
        ERROR_RETURN(vkCreateImageView(_device, &createInfo, nullptr, &_swapchainImageViews[i]));
    }

    return result;
}

/// @brief Create a swapchain and add it to the current device
/// @return VkResult
VkResult VkSwapchainClass::_createSwapchain() {
    DEBUG_MSG("\tCreating a swapchain...");
    VkResult result;

    // Check for swapchain support details in the current device
    SwapchainSupportDetails swapchainSupport = _querySwapChainSupport(_physicalDevice);

    // Set format, present mode, and extent
    VkSurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapchainSupport.formats);
    VkPresentModeKHR presentMode = _chooseSwapPresentMode(swapchainSupport.presentModes);
    VkExtent2D extent = _chooseSwapExtent(swapchainSupport.capabilities);

    // Get the number of images to be in the swapchain
    uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
    if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
        imageCount = swapchainSupport.capabilities.maxImageCount;
    }

    // Swapchain creator struct
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Set indicies
    QueueFamilyIndices indices = _findQueueFamilies(_physicalDevice);;
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

    createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Create a swapchain
    ERROR_RETURN(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain));

    // Map the swapchain images in memory
    vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, nullptr);
    _swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, _swapchainImages.data());

    // Set the format and extent
    _swapchainImageFormat = surfaceFormat.format;
    _swapchainExtent = extent;

    ERROR_RETURN(_createImageViews());

    return result;
}

/// @brief Destroys resources used by any swapchains
void VkSwapchainClass::_cleanupSwapchain() {
    DEBUG_MSG("\tCleaning up swapchain...");

    // Destroy buffers
    for (size_t i = 0; i < _swapchainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(_device, _swapchainFramebuffers[i], nullptr);
    }

    // Destroy image views
    for (size_t i = 0; i < _swapchainImageViews.size(); i++) {
        vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
    }

    // Destroy swapchain
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);
}

/// @brief Rebuilds the swapchain for handling window resizes and minimization
/// @return VkResult
VkResult VkSwapchainClass::_recreateSwapchain() {
    DEBUG_MSG("\tRecreating swapchain...");
    VkResult result;

    // Handle minimization
    int width = 0, height = 0;
    glfwGetFramebufferSize(_window, &width, &height);
    // Idle while the window is minimized
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(_window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(_device);

    _cleanupSwapchain();

    ERROR_RETURN(_createSwapchain());
    ERROR_RETURN(_createImageViews());
    ERROR_RETURN(_createFramebuffers());

    return result;
}

/// @brief Adds render pass info to the current device
/// @return VkResult
VkResult VkSwapchainClass::_createRenderPass() {
    DEBUG_MSG("\tCreating render pass...");

    // Create a single color buffer attachment
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = _swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // This is a subpass reference
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpasses are rendering operations that depend on the constants of framebuffers in previous stages
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // Set subpass dependencies
    VkSubpassDependency dependency{};
    // Specify indicies of the dependency and the dependant subpass
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    // Specify what operations to wait on and the stages in which these occur
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Attach other info to the render pass configuration
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    // Add the dependencies to the render pass
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    return vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass);
}

/// @brief Cleans up render pass related data
void VkSwapchainClass::_cleanupRenderPass() {
    DEBUG_MSG("\tCleaning up render pass...");
    vkDestroyRenderPass(_device, _renderPass, nullptr);
}