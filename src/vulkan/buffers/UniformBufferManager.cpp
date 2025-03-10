#include "UniformBufferManager.hpp"
/**
 * @brief Uniform buffer manager definitions.
 * @author Ray Richter
 */

/// @section Constructors

UniformBufferManager::UniformBufferManager(DeviceManager *pDeviceMgr, const size_t &maxFIF) {
    DEBUG_MSG("Called UniformBufferManager init constructor");
    VK_CHECK(init_(pDeviceMgr, maxFIF));
}

UniformBufferManager::UniformBufferManager() {
    DEBUG_MSG("Called UniformBufferManager default constructor");
}

UniformBufferManager::~UniformBufferManager() {
    DEBUG_MSG("Called UniformBufferManager deconstructor");
    for (size_t i = 0; i < maxFIF_; i++) {
        vkDestroyBuffer(pDeviceMgr_->getDevice(), uniformBuffers_[i], nullptr);
        vkFreeMemory(pDeviceMgr_->getDevice(), uniformBuffersMemory_[i], nullptr);
    }
}

/// @section Public Member Functions

VkResult UniformBufferManager::init(DeviceManager *pDeviceMgr, const size_t &maxFIF) {
    DEBUG_MSG("Called UniformBufferManager initalizer");
    return init_(pDeviceMgr, maxFIF);
}

VkResult UniformBufferManager::update(const uint32_t &currentImage, const VkExtent2D &currentExtent) {
    // Use crono to rotate the geometry 90*/s despite framerate
    static auto startTime = std::chrono::high_resolution_clock::now();

    // Compare start time with current time to get the amount we want to rotate
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    // Update the models rotation and perspective
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), 
        time * glm::radians(90.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f));

    // We decide to look at the model from above at 45*
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Use a perspective projection with a 45* vertical FOV
    ubo.proj = glm::perspective(glm::radians(45.0f), 
        currentExtent.width / (float) currentExtent.height, 
        0.1f, 10.0f);

    // Convert from OpenGL cords to Vulkan cords (Flip Y axis)
    ubo.proj[1][1] *= -1;

    // Copy data from the object to the current uniform buffer
    memcpy(uniformBuffersMapped_[currentImage], &ubo, sizeof(ubo));

    return VK_SUCCESS;
}

/// @section Private Member Functions

VkResult UniformBufferManager::init_(DeviceManager *pDeviceMgr, const size_t &maxFIF) {
    DEBUG_MSG("\tInitalizing UniformBufferManager...");
    VkResult result;
    pDeviceMgr_ = pDeviceMgr;
    maxFIF_ = maxFIF;

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    
    // Set the buffers to their maximum size
    uniformBuffers_.resize(maxFIF_);
    uniformBuffersMemory_.resize(maxFIF_);
    uniformBuffersMapped_.resize(maxFIF_);

    // Create and map each buffer
    for (size_t i = 0; i < maxFIF_; i++) {
        result = createBuffer_(bufferSize, 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            uniformBuffers_[i], 
            uniformBuffersMemory_[i]);

        result = vkMapMemory(pDeviceMgr_->getDevice(), uniformBuffersMemory_[i], 0, bufferSize, 0, &uniformBuffersMapped_[i]);
        if (result != VK_SUCCESS) return result;
    }

    DEBUG_MSG("\tInit done!");
    return result;
}

VkResult UniformBufferManager::createBuffer_(
const VkDeviceSize &size, 
const VkBufferUsageFlags &usage, 
const VkMemoryPropertyFlags &properties,
VkBuffer &buffer,
VkDeviceMemory &bufferMemory) const {
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
    result = vkCreateBuffer(pDeviceMgr_->getDevice(), &bufferInfo, nullptr, &buffer);
    if (result != VK_SUCCESS) return result;
    // Get the memory requirements of the VkBuffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(pDeviceMgr_->getDevice(), buffer, &memRequirements);
    // Configure the memory allocation
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, properties);
    result = vkAllocateMemory(pDeviceMgr_->getDevice(), &allocInfo, nullptr, &bufferMemory);
    if (result != VK_SUCCESS) return result;
    vkBindBufferMemory(pDeviceMgr_->getDevice(), buffer, bufferMemory, 0);
    DEBUG_MSG("\t\tBuffer creation done!");
    return result;
}

uint32_t UniformBufferManager::findMemoryType_(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties) const {
    // Get the gpu memory properties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pDeviceMgr_->getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) { return i; }}
    THROW_ERR("UBOManager ERROR: Failed to find suitable memory type!");
}