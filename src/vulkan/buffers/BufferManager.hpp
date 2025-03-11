#pragma once
/**
 * @brief A class for managing Vulkan buffers
 * @author Ray Richter
 */
#include <vulkan/vulkan.h>
#include <typeinfo>
#include "CommandPoolManager.hpp"
#include "raysDebugHelper.hpp"

template <typename Buffer_t>
class BufferManager {
public:
    BufferManager(CommandPoolManager *pCmdMgr, const std::vector<Buffer_t> &in) {
        DEBUG_MSG("Called BufferManager init constructor");
        VK_CHECK(init_(pCmdMgr, in)); }
    BufferManager() { DEBUG_MSG("Called BufferManager default constructor"); }
    ~BufferManager() { 
        DEBUG_MSG("Called BufferManager deconstructor");
        // vkDestroyBuffer(pCmdMgr_->getDevice(), buffer_, nullptr);
        // vkFreeMemory(pCmdMgr_->getDevice(), bufferMemory_, nullptr);
    }

    /// @brief Creates a staging buffer then moves the buffer to gpu and cpu memory.
    /// @return `VkResult`
    VkResult init(CommandPoolManager *pCmdMgr, const std::vector<Buffer_t> &in) {
        DEBUG_MSG("Called BufferManager init function");
        return init_(pCmdMgr, in); }

    VkResult cleanup() {
        vkDestroyBuffer(pCmdMgr_->getDevice(), buffer_, nullptr);
        vkFreeMemory(pCmdMgr_->getDevice(), bufferMemory_, nullptr); }

    /// @brief Creates a buffer.
    /// @param size Buffer size as `VkDeviceSize`.
    /// @param usage Buffer usage flags.
    /// @param properties Memory property flags.
    /// @param buffer Buffer as `VkBuffer`.
    /// @param bufferMemory Device memory as `VkDeviceMemory`.
    /// @return `VkResult`
    VkResult createBuffer(
        const VkDeviceSize &size, 
        const VkBufferUsageFlags &usage, 
        const VkMemoryPropertyFlags &properties,
        VkBuffer &buffer,
        VkDeviceMemory &bufferMemory
    ) {
        DEBUG_MSG("\t\tCreating a buffer...");
        VkResult result;

        // Buffer configuration
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        // Set buffer size in bytes
        bufferInfo.size = size;
        // Set buffer usage. Multiple uses can be specified with bitwise or
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        result = vkCreateBuffer(pCmdMgr_->getDevice(), &bufferInfo, nullptr, &buffer);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tGetting buffer memory requirements...");
        // Get the memory requirements of the VkBuffer
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(pCmdMgr_->getDevice(), buffer, &memRequirements);

        // Configure the memory allocation
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, properties);

        DEBUG_MSG("\t\tAllocating memory...");
        result = vkAllocateMemory(pCmdMgr_->getDevice(), &allocInfo, nullptr, &bufferMemory);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tBinding buffer memory...");
        result = vkBindBufferMemory(pCmdMgr_->getDevice(), buffer, bufferMemory, 0);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\tBuffer creation done!");
        return result;
    }
    /// @brief Copies a buffer.
    /// @param srcBuffer Source `VkBuffer`.
    /// @param dstBuffer Destination `VkBuffer`.
    /// @param size Buffer size as `VkDeviceSize`.
    /// @return `VkResult`
    VkResult copyBuffer(
        const VkBuffer &srcBuffer, 
        const VkBuffer &dstBuffer, 
        const VkDeviceSize size
    ) const {
        DEBUG_MSG("\t\tCopying a buffer...");
        VkResult result;

        // Configure a command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = pCmdMgr_->getPool();
        allocInfo.commandBufferCount = 1;

        DEBUG_MSG("\t\tAllocating command buffer...");
        // Allocate the commands
        VkCommandBuffer commandBuffer;
        result = vkAllocateCommandBuffers(pCmdMgr_->getDevice(), &allocInfo, &commandBuffer);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tRecording command buffer...");
        // Start recording the command buffer
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tInit copy command...");
        // Initiate a coppy command
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        DEBUG_MSG("\t\tEnd command buffer recording...");
        // End the command buffer recording
        result = vkEndCommandBuffer(commandBuffer);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tSubmitting command buffer...");
        // No events to wait for, just copy the memory
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        result = vkQueueSubmit(pCmdMgr_->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tWaiting for command buffer...");
        /// TODO: Add fencing to queue multiple memory transfers at once
        // Wait for the transfer
        result = vkQueueWaitIdle(pCmdMgr_->getGraphicsQueue());
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\t\tCleaning command buffer...");
        // Cleanup command buffer
        vkFreeCommandBuffers(pCmdMgr_->getDevice(), pCmdMgr_->getPool(), 1, &commandBuffer);

        DEBUG_MSG("\t\tCopy done!");
        return result;
    }

    VkResult remake() { return init_(pCmdMgr_, bufferList_); }
    VkBuffer getBuffer() const { return buffer_; }
    inline size_t size() const { return bufferList_.size(); }

private:
    std::vector<Buffer_t>   bufferList_; 
    VkBuffer                buffer_;
    VkDeviceMemory          bufferMemory_;
    VkDeviceSize            bufferSize_;
    CommandPoolManager*     pCmdMgr_;

    VkResult init_(CommandPoolManager *pCmdMgr, const std::vector<Buffer_t> &in) {
        DEBUG_MSG("\tInitalizing BufferManager with type: " << typeid(Buffer_t).name());
        bufferSize_ = ARRAY_SIZE(in);
        bufferList_.resize(in.size());
        bufferList_ = in;
        pCmdMgr_ = pCmdMgr;
        VkResult result;
        DEBUG_MSG("\tBuffer size: " << bufferSize_ << " bytes");
    
        // Create staging buffer in high performance memory
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        DEBUG_MSG("\tCreating a staging buffer...");
        result = createBuffer(bufferSize_, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);
        if (result != VK_SUCCESS) return result;

        // Copy the buffer into CPU accessable memory
        DEBUG_MSG("\tCopying buffer to CPU accessable memory...");
        void* data;
        vkMapMemory(pCmdMgr_->getDevice(), stagingBufferMemory, 0, bufferSize_, 0, &data);
        memcpy(data, bufferList_.data(), (size_t) bufferSize_);
        vkUnmapMemory(pCmdMgr_->getDevice(), stagingBufferMemory);

        DEBUG_MSG("\tCreating a buffer for GPU memory...");
        result = createBuffer(bufferSize_, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            buffer_,
            bufferMemory_);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\tCopying buffer to GPU memory...");
        // Copy the buffer into GPU memory
        result = copyBuffer(stagingBuffer, buffer_, bufferSize_);
        if (result != VK_SUCCESS) return result;

        DEBUG_MSG("\tRemoving staging buffer...");
        // Destroy the temporary buffer
        vkDestroyBuffer(pCmdMgr_->getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(pCmdMgr_->getDevice(), stagingBufferMemory, nullptr);

        return result;
    }
    uint32_t findMemoryType_(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties) const {
        // Get the gpu memory properties
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(pCmdMgr_->getPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && 
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { 
                    DEBUG_MSG("\t\t\tFound memory type: " << i);
                    return i; }}
        THROW_ERR("BufferManager ERROR: Failed to find suitable memory type!");
    }
};