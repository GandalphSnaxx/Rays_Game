#include "FrameBufferManager.hpp"
/**
 * @brief Frame buffer manager definitions.
 * @author Ray Richter
 */

/// @section Constructors

FrameBufferManager::FrameBufferManager(RenderPassManager *pRdPassMgr) {
    DEBUG_MSG("Called FrameBufferManager init constructor");
    VK_CHECK(init_(pRdPassMgr));
}

FrameBufferManager::FrameBufferManager() {
    DEBUG_MSG("Called FrameBufferManager default constructor");
}

FrameBufferManager::~FrameBufferManager() {
    DEBUG_MSG("Called FrameBufferManager deconstructor");
}

/// @section Public Member Functions

VkResult FrameBufferManager::init(RenderPassManager *pRdPassMgr) {
    DEBUG_MSG("Called FrameBufferManager init function");
    return init_(pRdPassMgr);
}

/// @section Private Member Functions

VkResult FrameBufferManager::init_(RenderPassManager *pRdPassMgr) {
    DEBUG_MSG("\tInitalizing FrameBufferManager...");
    VkResult result;
    pRenderPassManager_ = pRdPassMgr;

    // Resize the container to hold all frame buffers
    framebuffers_.resize(pRenderPassManager_->getScImageViews().size());

    // Iterate through image views and create frame buffers for them
    for (size_t i = 0; i < pRenderPassManager_->getScImageViews().size(); i++) {
        VkImageView attachments[] = {
            pRenderPassManager_->getScImageViews()[i]
        };
    
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = pRenderPassManager_->getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments    = attachments;
        framebufferInfo.width           = pRenderPassManager_->getScExtent().width;
        framebufferInfo.height          = pRenderPassManager_->getScExtent().height;
        framebufferInfo.layers          = 1;
    
        result = vkCreateFramebuffer(pRenderPassManager_->getDevice(), &framebufferInfo, nullptr, &framebuffers_[i]);
        if (result != VK_SUCCESS) return result;
    }

    DEBUG_MSG("\tInit done!");
    return result;
}