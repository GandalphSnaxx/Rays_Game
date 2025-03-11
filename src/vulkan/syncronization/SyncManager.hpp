#pragma once
/**
 * @brief Syncronization manager for Vulkan.
 * @author Ray Richter
 */
#include "vulkan/swapchain/SwapchainManager.hpp"
#include "raysDebugHelper.hpp"

class SyncManager {
public:
    SyncManager(SwapchainManager*, const size_t&);
    SyncManager();
    ~SyncManager();

    VkResult init(SwapchainManager*, const size_t&);
    VkResult cleanup();
    /// @brief Waits for fences
    /// @param fenceCount 
    /// @param fenceIndex 
    /// @param waitAll 
    /// @param timeout 
    /// @return `VkResult`
    VkResult wait(const uint32_t&, const size_t&, const VkBool32&, const uint64_t&) const;
    /// @brief Gets the next image index
    /// @param timeout 
    /// @param index 
    /// @param fence 
    /// @param imageIdx The image index pointer that gets updated.
    /// @return `VkResult`
    VkResult getNextImage(const uint64_t&, const size_t&, const VkFence&, uint32_t*) const;
    VkResult resetFences(const uint32_t&, const size_t&);
    // void createSyncObjects(VkDevice device, size_t maxFramesInFlight);
    const std::vector<VkSemaphore>& getImageAvailableSemaphores() const { return imageAvailableSemaphores_;     }
    const VkSemaphore getImageAvailableSemaphore(const size_t &i) const { return imageAvailableSemaphores_[i];  }
    const std::vector<VkSemaphore>& getRenderFinishedSemaphores() const { return renderFinishedSemaphores_;     }
    const VkSemaphore getRenderFinishedSemaphore(const size_t &i) const { return renderFinishedSemaphores_[i];  }
    const std::vector<VkFence>&     getInFlightFences          () const { return inFlightFences_;               }
    const VkFence     getInFlightFence          (const size_t &i) const { return inFlightFences_[i];            }

private:
    std::vector<VkSemaphore>    imageAvailableSemaphores_;
    std::vector<VkSemaphore>    renderFinishedSemaphores_;
    std::vector<VkFence>        inFlightFences_;
    SwapchainManager*           pSwapMgr_;
    size_t                      maxFIF_;
    DeviceManager*              pDeviceMgr_;

    VkResult init_(SwapchainManager*, const size_t&);
};