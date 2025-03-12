#pragma once
/**
 * @brief Command pool manager for Vulkan.
 * @author Ray Richter
 */
#include <vulkan/vulkan.h>
#include "vulkan/render_pass/RenderPass.hpp"
#include "raysDebugHelper.hpp"

class CommandPoolManager {
public:
    CommandPoolManager(
        RenderPassManager *pRdPassMgr, 
        const size_t &maxFIF,
        const uint32_t &qFamilyIdx, 
        const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    CommandPoolManager();
    ~CommandPoolManager();

    VkResult init( // createCommandPool
        RenderPassManager *pRdPassMgr, 
        const size_t &maxFIF,
        const uint32_t &qFamilyIdx, 
        const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkResult cleanup();
    VkResult resetCB(const size_t &index, const VkCommandBufferResetFlagBits &flags = VkCommandBufferResetFlagBits(0)) 
    { vkResetCommandBuffer(commandBuffers_[index], flags); return VK_SUCCESS; }
    // VkResult recordCB(
    //     const uint32_t &currentFrame, 
    //     const uint32_t &imageIndex, 
    //     const VkFramebuffer &fb, 
    //     const VkPipeline &gfxPipeline);
    
    VkDevice         getDevice        () const { return pRdPassMgr_->getDevice();        }
    VkPhysicalDevice getPhysicalDevice() const { return pRdPassMgr_->getPhysicalDevice();}
    VkCommandPool    getPool          () const { return commandPool_;                    }
    std::vector<VkCommandBuffer>& getCmdBuffers() { return commandBuffers_;              }
    VkCommandBuffer  getCmdBuffer     (const size_t &i)const{ return  commandBuffers_[i];}
    // VkCommandBuffer* getPCmdBuffer    (const size_t &i)const{ return &commandBuffers_[i];}
    VkQueue          getGraphicsQueue () const { return pRdPassMgr_->getGraphicsQueue(); }
    VkQueue          getPresentQueue  () const { return pRdPassMgr_->getPresentQueue();  }

private:
    VkCommandPool   commandPool_;
    std::vector<VkCommandBuffer> commandBuffers_;
    RenderPassManager*  pRdPassMgr_;
    size_t          maxFIF_;

    VkResult init_(
        RenderPassManager *pRdPassMgr, 
        const size_t &maxFIF,
        const uint32_t &qFamilyIdx, 
        const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
};