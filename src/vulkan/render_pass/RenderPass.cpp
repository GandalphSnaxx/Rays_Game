#include "RenderPass.hpp"
/**
 * @file vulkan/render_pass/RenderPass.cpp
 * @author Ray Richter
 * @brief Render pass related definitions.
 */

/// @section Constructors

RenderPassManager::RenderPassManager(SwapchainManager *pSwapMgr) {
    DEBUG_MSG("Called RenderPassManager init constructor");
    VK_CHECK(init_(pSwapMgr));
}

RenderPassManager::RenderPassManager() {
    DEBUG_MSG("Called RenderPassManager default constructor");
}

RenderPassManager::~RenderPassManager() {
    DEBUG_MSG("Called RenderPassManager deconstructor");
    // vkDestroyRenderPass(pSwapMgr_->getDevice(), renderPass_, nullptr);
}

/// @section Public Member Functions

VkResult RenderPassManager::init(SwapchainManager *pSwapMgr) {
    DEBUG_MSG("Initalizing RenderPassManager with a function...");
    return init_(pSwapMgr);
}

VkResult RenderPassManager::cleanup() {
    vkDestroyRenderPass(pSwapMgr_->getDevice(), renderPass_, nullptr);
    return VK_SUCCESS;
}

/// @section Private Member Functions

VkResult RenderPassManager::init_(SwapchainManager *pSwapMgr) {
    DEBUG_MSG("\tInitalizing RenderPassManager...");
    pSwapMgr_ = pSwapMgr;

    // Create a list of subpasses
    /// TODO: Fix this:
    // Subpasses subpasses({
    //     {VK_FORMAT_UNDEFINED, swapchainImageFormats[0], VK_FORMAT_UNDEFINED, VK_FORMAT_UNDEFINED}
    // });

    /// TODO: Make a few structs for easier management
    std::vector<VkAttachmentDescription> attachments(1);
    attachments[0].format           = pSwapMgr_->getScImageFormat();
    attachments[0].samples          = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp           = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp          = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp    = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    std::vector<VkAttachmentReference> attachmentRefs(1);
    attachmentRefs[0].attachment    = 0;
    attachmentRefs[0].layout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    std::vector<VkSubpassDescription> subpass(1);
    subpass[0].pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass[0].colorAttachmentCount = 1;
    subpass[0].pColorAttachments    = attachmentRefs.data();

    std::vector<VkSubpassDependency> subpassDeps(1);
    // Specify indicies of the dependency and the dependant subpass
    subpassDeps[0].srcSubpass       = VK_SUBPASS_EXTERNAL;
    subpassDeps[0].dstSubpass       = 0;
    // Specify what operations to wait on and the stages in which these occur
    subpassDeps[0].srcStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDeps[0].srcAccessMask    = 0;
    subpassDeps[0].dstStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDeps[0].dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Configure render pass information
    VkRenderPassCreateInfo renderPassInfo{};
    attachmentCount_                = attachments.size();
    renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr; renderPassInfo.flags = 0; // Optional

    renderPassInfo.attachmentCount  = attachments.size();
    renderPassInfo.pAttachments     = attachments.data();
    renderPassInfo.subpassCount     = subpass.size();
    renderPassInfo.pSubpasses       = subpass.data();
    renderPassInfo.dependencyCount  = subpassDeps.size();
    renderPassInfo.pDependencies    = subpassDeps.data();

    DEBUG_MSG("\tSuccess!");
    return vkCreateRenderPass(pSwapMgr_->getDevice(), &renderPassInfo, nullptr, &renderPass_);
}

// /// @brief Render pass constructor. Adds render pass information to the current device
// VkRenderPassClass::VkRenderPassClass() {
//     DEBUG_MSG("Called VkRenderPassClass constructor");

//     // Create a single color buffer attachment
//     VkAttachmentDescription colorAttachment{};
//     colorAttachment.format = _swapchainImageFormat;
//     colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//     colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//     colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//     colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//     colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//     colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//     colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

//     // This is a subpass reference
//     VkAttachmentReference colorAttachmentRef{};
//     colorAttachmentRef.attachment = 0;
//     colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

//     // Subpasses are rendering operations that depend on the constants of framebuffers in previous stages
//     VkSubpassDescription subpass{};
//     subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//     subpass.colorAttachmentCount = 1;
//     subpass.pColorAttachments = &colorAttachmentRef;

//     // Set subpass dependencies
//     VkSubpassDependency dependency{};
//     // Specify indicies of the dependency and the dependant subpass
//     dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//     dependency.dstSubpass = 0;
//     // Specify what operations to wait on and the stages in which these occur
//     dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//     dependency.srcAccessMask = 0;
//     dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//     dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

//     // Attach other info to the render pass configuration
//     VkRenderPassCreateInfo renderPassInfo{};
//     renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//     renderPassInfo.attachmentCount = 1;
//     renderPassInfo.pAttachments = &colorAttachment;
//     renderPassInfo.subpassCount = 1;
//     renderPassInfo.pSubpasses = &subpass;

//     // Add the dependencies to the render pass
//     renderPassInfo.dependencyCount = 1;
//     renderPassInfo.pDependencies = &dependency;

//     VK_CHECK(vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass));
// }

// /// @brief Render pass deconstructor
// VkRenderPassClass::~VkRenderPassClass() {
//     DEBUG_MSG("Called VkRenderPassClass deconstructor");
//     vkDestroyRenderPass(_device, _renderPass, nullptr);
// }

// /// @brief Rebuilds the swapchain for handling window resizes and minimization
// /// @return VkResult
// VkResult VkRenderPassClass::_recreateSwapchain() {
//     VkResult result;

//     // Handle minimization
//     int width = 0, height = 0;
//     glfwGetFramebufferSize(_window, &width, &height);
//     // Idle while the window is minimized
//     while (width == 0 || height == 0) {
//         glfwGetFramebufferSize(_window, &width, &height);
//         glfwWaitEvents();
//     }

//     vkDeviceWaitIdle(_device);

//     _cleanupSwapchain();

//     ERROR_RETURN(_createSwapchain());
//     ERROR_RETURN(_createImageViews());
//     ERROR_RETURN(_createFramebuffers());

//     return result;
// }

// /// @brief Creates a frame buffer for every image view
// /// @return VkResult
// VkResult VkRenderPassClass::_createFramebuffers() {
//     VkResult result;
//     // Resize the container to hold all frame buffers
//     _swapchainFramebuffers.resize(_swapchainImageViews.size());

//     // Iterate through image views and create frame buffers for them
//     for (size_t i = 0; i < _swapchainImageViews.size(); i++) {
//         VkImageView attachments[] = {
//             _swapchainImageViews[i]
//         };
    
//         VkFramebufferCreateInfo framebufferInfo{};
//         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         framebufferInfo.renderPass = _renderPass;
//         framebufferInfo.attachmentCount = 1;
//         framebufferInfo.pAttachments = attachments;
//         framebufferInfo.width = _swapchainExtent.width;
//         framebufferInfo.height = _swapchainExtent.height;
//         framebufferInfo.layers = 1;
    
//         ERROR_RETURN(vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapchainFramebuffers[i]));
//     }

//     return result;
// }