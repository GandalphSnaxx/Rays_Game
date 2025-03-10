#include "CommandPoolManager.hpp"
/**
 * @brief Command pool manager definitions.
 * @author Ray Richter
 */

/// @section Constructors

CommandPoolManager::CommandPoolManager(
RenderPassManager *pRdPassMgr, 
const size_t &maxFIF,
const uint32_t &qFamilyIdx, 
const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
    DEBUG_MSG("Called CommandPoolManager init constructor");
    VK_CHECK(init_(pRdPassMgr, maxFIF, qFamilyIdx, flags));
}

CommandPoolManager::CommandPoolManager() {
    DEBUG_MSG("Called CommandPoolManager default constructor");
}

CommandPoolManager::~CommandPoolManager() {
    DEBUG_MSG("Called CommandPoolManager deconstructor");
    vkDestroyCommandPool(pRdPassMgr_->getDevice(), commandPool_, nullptr);
}

/// @section Public Member Functions

VkResult CommandPoolManager::init(
RenderPassManager *pRdPassMgr, 
const size_t &maxFIF,
const uint32_t &qFamilyIdx, 
const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
    DEBUG_MSG("Initalizing CommandPoolManager with a function...");
    return init_(pRdPassMgr, maxFIF, qFamilyIdx, flags);
}

/// @section Private Member Functions

VkResult CommandPoolManager::init_(
RenderPassManager *pRdPassMgr, 
const size_t &maxFIF,
const uint32_t &qFamilyIdx, 
const VkCommandPoolCreateFlagBits &flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
    DEBUG_MSG("\tInitalizing CommandPoolManager...");
    pRdPassMgr_ = pRdPassMgr; // Link a device manager to this class
    maxFIF_ = maxFIF;
    VkResult result;

    // Two possible flags for command pools: VK_COMMAND_POOL_CREATE_TRANSIENT_BIT and VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext              = nullptr; // Optional
    poolInfo.flags              = flags;
    poolInfo.queueFamilyIndex   = qFamilyIdx;

    result = vkCreateCommandPool(pRdPassMgr_->getDevice(), &poolInfo, nullptr, &commandPool_);
    if (result != VK_SUCCESS) return result;

    commandBuffers_.resize(maxFIF_);
    // Specify the command pool and number of buffers
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool_;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers_.size();

    result = vkAllocateCommandBuffers(pRdPassMgr_->getDevice(), &allocInfo, commandBuffers_.data());
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tInit done!");
    return result;
}