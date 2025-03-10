#pragma once
/**
 * @brief Uniform buffer manager.
 * @author Ray Richter
 */
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include "vulkan/devices/DeviceManager.hpp"
#include "raysDebugHelper.hpp"

// A description of the types of resources to be accessed by the pipeline
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class UniformBufferManager {
public:
    UniformBufferManager(DeviceManager *pDeviceMgr, const size_t &maxFIF);
    UniformBufferManager();
    ~UniformBufferManager();

    /// @brief Initalizes uniform buffers and maps their memory.
    /// @param pDeviceMgr A pointer to a device manager to add uniform buffers to.
    /// @param maxFIF Maximum frames in flight.
    /// @return `VkResult`
    VkResult init(DeviceManager *pDeviceMgr, const size_t &maxFIF);
    VkResult update(const uint32_t&, const VkExtent2D&);

    std::vector<VkBuffer>& getBuffers()       { return uniformBuffers_;    }
    VkBuffer getBuffer(const size_t &i) const { return uniformBuffers_[i]; }
    VkDevice getDevice() const { return pDeviceMgr_->getDevice(); }
    size_t   getMaxFIF() const { return maxFIF_; }

private:
    std::vector<VkBuffer>       uniformBuffers_;
    std::vector<VkDeviceMemory> uniformBuffersMemory_;
    // std::vector<BufferManager>  uniformBuffers_;
    std::vector<void*>          uniformBuffersMapped_;
    DeviceManager*              pDeviceMgr_;
    size_t                      maxFIF_;

    VkResult init_(DeviceManager *pDeviceMgr, const size_t &maxFIF);
    VkResult createBuffer_(
        const VkDeviceSize &size, 
        const VkBufferUsageFlags &usage, 
        const VkMemoryPropertyFlags &properties,
        VkBuffer &buffer,
        VkDeviceMemory &bufferMemory) const;
    uint32_t findMemoryType_(const uint32_t &typeFilter, const VkMemoryPropertyFlags &properties) const;
};