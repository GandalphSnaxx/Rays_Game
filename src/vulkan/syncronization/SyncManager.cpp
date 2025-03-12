#include "SyncManager.hpp"
/**
 * @brief Syncronization manager definitions.
 * @author Ray Richter
 */

/// @section Constructors

SyncManager::SyncManager(SwapchainManager *pSwapMgr, const size_t &maxFIF) {
    DEBUG_MSG("Called SyncManager init constructor");
    VK_CHECK(init_(pSwapMgr, maxFIF));
}

SyncManager::SyncManager() {
    DEBUG_MSG("Called SyncManager default constructor");
}

SyncManager::~SyncManager() {
    DEBUG_MSG("Called SyncManager deconstructor");
    // for (size_t i = 0; i < maxFIF_; i++) {
    //     vkDestroySemaphore(pDeviceMgr_->getDevice(), imageAvailableSemaphores_[i], nullptr);
    //     vkDestroySemaphore(pDeviceMgr_->getDevice(), renderFinishedSemaphores_[i], nullptr);
    //     vkDestroyFence    (pDeviceMgr_->getDevice(), inFlightFences_[i],           nullptr);
    // }
}

/// @section Public Member Functions

VkResult SyncManager::init(SwapchainManager *pSwapMgr, const size_t &maxFIF) {
    DEBUG_MSG("Called SyncManager init function...");
    return init_(pSwapMgr, maxFIF);
}

VkResult SyncManager::cleanup() {
    for (size_t i = 0; i < maxFIF_; i++) {
        vkDestroySemaphore(pDeviceMgr_->getDevice(), imageAvailableSemaphores_[i], nullptr);
        vkDestroySemaphore(pDeviceMgr_->getDevice(), renderFinishedSemaphores_[i], nullptr);
        vkDestroyFence    (pDeviceMgr_->getDevice(), inFlightFences_[i],           nullptr);
    }
    return VK_SUCCESS;
}

VkResult SyncManager::wait(
const uint32_t &fenceCount, 
const size_t &fenceIndex, 
const VkBool32 &waitAll, 
const uint64_t &timeout) const {
    vkWaitForFences(
        pSwapMgr_->getDevice(), 
        fenceCount, 
        &inFlightFences_[fenceIndex], 
        waitAll, 
        timeout);
    return VK_SUCCESS;
}

VkResult SyncManager::getNextImage(const uint64_t &timeout, const size_t &index, const VkFence &fence, uint32_t *imageIdx) const {
    return vkAcquireNextImageKHR(
        pSwapMgr_->getDevice(), 
        pSwapMgr_->getSwapchain(), 
        timeout, 
        imageAvailableSemaphores_[index], 
        fence, 
        imageIdx);
}

VkResult SyncManager::resetFences(const uint32_t &fenceCount, const size_t &index) {
    vkResetFences(pSwapMgr_->getDevice(), fenceCount, &inFlightFences_[index]);
    return VK_SUCCESS;
}

/// @section Private Member Functions

VkResult SyncManager::init_(SwapchainManager *pSwapMgr, const size_t &maxFIF) {
    DEBUG_MSG("\tInitalizing SyncManager...");
    VkResult result;
    pSwapMgr_ = pSwapMgr;
    maxFIF_ = maxFIF;
    pDeviceMgr_ = pSwapMgr_->getPDeviceMgr();

    imageAvailableSemaphores_.resize(maxFIF_);
    renderFinishedSemaphores_.resize(maxFIF_);
    inFlightFences_.resize(maxFIF_);

    // Create a semaphore
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    // Create a fence
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // Add a signal saying the first frame can be drawn
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Build the semaphores and fences
    for (size_t i = 0; i < maxFIF_; i++) {
        result = vkCreateSemaphore(pSwapMgr_->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores_[i]);
        if (result != VK_SUCCESS) return result;
        result = vkCreateSemaphore(pSwapMgr_->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores_[i]);
        if (result != VK_SUCCESS) return result;
        result = vkCreateFence    (pSwapMgr_->getDevice(), &fenceInfo,     nullptr, &inFlightFences_[i]);
        if (result != VK_SUCCESS) return result;
    }

    DEBUG_MSG("\tInit done!");
    return result;
}