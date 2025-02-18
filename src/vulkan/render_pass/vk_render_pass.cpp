#include "vk_render_pass.hpp"

VkRenderPassClass::VkRenderPassClass() {}

VkRenderPassClass::~VkRenderPassClass() {
    vkDestroyRenderPass(device->getDevice(), renderPass, nullptr);
}

/// @brief Configure a render pass
/// @param pDevice The device the render passes apply to
/// @param pSwapchain The swapchain render passes apply to
/// @return VkResult
VkResult VkRenderPassClass::init(VkDevicesClass *pDevice, VkSwapchainClass *pSwapchain) {
    // Link this class to the current device and swapchain
    device = pDevice;
    swapchain = pSwapchain;

    // Create a single color buffer attachment
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain->getImageFormat();
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

    return vkCreateRenderPass(device->getDevice(), &renderPassInfo, nullptr, &renderPass);
}