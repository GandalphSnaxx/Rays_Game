#include "memory_mgmt.hpp"

VkMemoryClass::VkMemoryClass() {

}

VkMemoryClass::~VkMemoryClass() {

}

/// @brief Buffers in Vulkan are regions of memory used for storing arbitrary data that can be read by the graphics card
/// @param size The size of the buffer to be created
/// @param usage Buffer usage flag bits
/// @param properties Memory flags in a bitfield
/// @param buffer A pointer to the cpu buffer
/// @param bufferMemory A pointer to the gpu buffer memory
void VkMemoryClass::_createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    // Buffer configuration
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // Set buffer size in bytes
    bufferInfo.size = size;
    // Set buffer usage. Multiple uses can be specified with bitwise or
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer));

    // Get the memory requirements of the VkBuffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

    // Configure the memory allocation
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _findMemoryType(memRequirements.memoryTypeBits, properties);

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(_device, buffer, bufferMemory, 0);
}

/// @brief Allocate a temporary command buffer for memory transfer commands
/// @param srcBuffer Source buffer
/// @param dstBuffer Destination buffer
/// @param size Buffer size
void VkMemoryClass::_copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    // Configure a command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    // Allocate the commands
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    // Start recording the command buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // Initiate a coppy command
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // End the command buffer recording
    vkEndCommandBuffer(commandBuffer);

    // No events to wait for, just copy the memory
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    
    // TODO: Add fencing to queue multiple memory transfers at once
    // Wait for the transfer
    vkQueueWaitIdle(_graphicsQueue);

    // Cleanup command buffer
    vkFreeCommandBuffers(_device, commandPool, 1, &commandBuffer);
}