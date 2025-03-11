#pragma once
/**
 * @brief Frame buffer manager.
 * @author Ray Richter
 */
#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan/render_pass/RenderPass.hpp"
#include "raysDebugHelper.hpp"

class FrameBufferManager {
public:
    FrameBufferManager(RenderPassManager *pRdPassMgr);
    FrameBufferManager();
    ~FrameBufferManager();

    VkResult init(RenderPassManager *pRdPassMgr);

    VkResult remake() { return init_(pRenderPassManager_); }
    std::vector<VkFramebuffer>& getFramebuffers() { return framebuffers_; }
    VkFramebuffer getFramebuffer(const size_t &i) const { return framebuffers_[i]; }
    void cleanupFramebuffers();

private:
    std::vector<VkFramebuffer>  framebuffers_;
    RenderPassManager*          pRenderPassManager_;

    VkResult init_(RenderPassManager *pRdPassMgr);
};
