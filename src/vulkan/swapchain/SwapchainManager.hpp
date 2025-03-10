#pragma once
/**
 * @file vulkan/swapchain/SwapchainManager.hpp
 * @author Ray Richter
 * @brief SwapchainManager class declaration.
 */
#include "vulkan/devices/DeviceManager.hpp"
#include "vulkan/surface/SurfaceManager.hpp"
#include "QueueFamilyIndices.hpp"
#include <algorithm> // Necessary for std::clamp
#include "raysDebugHelper.hpp"

class SwapchainManager {
public:
    SwapchainManager (DeviceManager*);
    SwapchainManager ();
    ~SwapchainManager();

    VkResult init(DeviceManager*);
    VkResult remake();

    VkSwapchainKHR                  getSwapchain     () const { return swapchain_;            }
    VkFormat                        getScImageFormat () const { return swapchainImageFormat_; }
    VkExtent2D                      getScExtent      () const { return swapchainExtent_;      }
    const std::vector<VkImage>&     getScImages      () const { return swapchainImages_;      }
    const std::vector<VkImageView>& getScImageViews  () const { return swapchainImageViews_;  }
    QueueFamilyIndices              getQFamIdx       () const { return queueFamilyIndices_;   }
    VkPhysicalDevice                getPhysicalDevice() const { return pDeviceMgr_->getPhysicalDevice(); }
    VkDevice                        getDevice        () const { return pDeviceMgr_->getDevice();         }
    VkQueue                         getGraphicsQueue () const { return pDeviceMgr_->getGraphicsQueue();  }
    VkQueue                         getPresentQueue  () const { return pDeviceMgr_->getPresentQueue();   }
    VkInstance                      getInstance      () const { return pDeviceMgr_->getInstance();}
    bool                            validLayers      () const { return pDeviceMgr_->vLayersEn();  }
    VkSurfaceKHR                    getSurface       () const { return pDeviceMgr_->getSurface(); }
    // VkInstance                   getInstance      () const { return pDeviceMgr_->getInstance();}
    GLFWwindow*                     getPWindow       () const { return pDeviceMgr_->getPWindow(); }
    size_t                          getMaxFIF        () const { return pDeviceMgr_->getMaxFIF();  }

private:
    struct SupportDetails_ { public:
        SupportDetails_(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) 
        { VK_CHECK(query(physicalDevice, surface)); }
        SupportDetails_() {}

        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;

        VkResult query(const VkPhysicalDevice&, const VkSurfaceKHR&);
    };

    VkSwapchainKHR              swapchain_;
    VkFormat                    swapchainImageFormat_;
    VkExtent2D                  swapchainExtent_;
    // VkDevice                    device_; // Copy of VkDevice for deconstruction
    // VkPhysicalDevice            physicalDevice_;
    std::vector<VkImage>        swapchainImages_;
    std::vector<VkImageView>    swapchainImageViews_;
    QueueFamilyIndices          queueFamilyIndices_;
    DeviceManager*              pDeviceMgr_;
    // VkCommandPool               commandPool_;
    // std::vector<VkFramebuffer>  swapchainFramebuffers_; // Moved to buffer manager

    VkResult            init_                   (DeviceManager*);
    void                cleanupSwapchain_       ();
    VkSurfaceFormatKHR  chooseSwapSurfaceFormat_(const std::vector<VkSurfaceFormatKHR>&);
    VkPresentModeKHR    chooseSwapPresentMode_  (const std::vector<VkPresentModeKHR>&);
    VkExtent2D          chooseSwapExtent_       (const VkSurfaceCapabilitiesKHR&, GLFWwindow*);
    // VkResult            createCommandPool_      (const QueueFamilyIndices_ &indices);
};