#pragma once
/**
 * @file vulkan/render_pass/RenderPass.hpp
 * @author Ray Richter
 * @brief Render pass related data and functions for Vulkan.
 */
#include "vulkan/swapchain/SwapchainManager.hpp"
#include "raysDebugHelper.hpp"

/// @brief A class for handling render pass related variables and functions
class RenderPassManager {
public:
    RenderPassManager(SwapchainManager *pSwapMgr);
    RenderPassManager();
    ~RenderPassManager();
    // Delete the default constructor to prevent its use
    // RenderPassManager::RenderPassManager() = delete;

    VkResult init(SwapchainManager *pSwapMgr);
    VkResult cleanup();

    VkRenderPass                    getRenderPass     () const { return renderPass_;                   }
    uint32_t                        getAttachmentCount() const { return attachmentCount_;              }
    VkSwapchainKHR                  getSwapchain      () const { return pSwapMgr_->getSwapchain();     }
    VkFormat                        getScImageFormat  () const { return pSwapMgr_->getScImageFormat(); }
    VkExtent2D                      getScExtent       () const { return pSwapMgr_->getScExtent();      }
    const std::vector<VkImage>&     getScImages       () const { return pSwapMgr_->getScImages();      }
    VkImage              getScImage(const size_t &index) const { return pSwapMgr_->getScImage(index);  }
    VkImageView      getScImageView(const size_t &index) const { return pSwapMgr_->getScImageView(index); }
    const std::vector<VkImageView>& getScImageViews   () const { return pSwapMgr_->getScImageViews();  }
    QueueFamilyIndices              getQFamIdx        () const { return pSwapMgr_->getQFamIdx();       }
    VkPhysicalDevice                getPhysicalDevice () const { return pSwapMgr_->getPhysicalDevice();}
    VkDevice                        getDevice         () const { return pSwapMgr_->getDevice();        }
    VkQueue                         getGraphicsQueue  () const { return pSwapMgr_->getGraphicsQueue(); }
    VkQueue                         getPresentQueue   () const { return pSwapMgr_->getPresentQueue();  }
    VkInstance                      getInstance       () const { return pSwapMgr_->getInstance();      }
    bool                            validLayers       () const { return pSwapMgr_->validLayers();      }
    VkSurfaceKHR                    getSurface        () const { return pSwapMgr_->getSurface();       }
    GLFWwindow*                     getPWindow        () const { return pSwapMgr_->getPWindow();       }
    size_t                          getMaxFIF         () const { return pSwapMgr_->getMaxFIF();        }
    size_t                          getImageSize      () const { return pSwapMgr_->getImageSize();     }
    size_t                          getViewsSize      () const { return pSwapMgr_->getViewsSize();     }

private:
    VkRenderPass        renderPass_;
    SwapchainManager*   pSwapMgr_;
    uint32_t            attachmentCount_;

    VkResult init_(SwapchainManager *pSwapMgr); // Internal initalizer
};