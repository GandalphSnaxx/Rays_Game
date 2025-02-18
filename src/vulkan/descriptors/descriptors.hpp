#ifndef VULKAN_DESCRIPTORS_HPP__
#define VULKAN_DESCRIPTORS_HPP__

#include "overhead.hpp"
#include <vector>

class VkDescriptorClass : virtual public VkOverheadClass {
    public:
    VkDescriptorClass();
    ~VkDescriptorClass();
    // Remove the default constructor so it is not accidentally used
    // VkDescriptorClass::VkDescriptorClass() = delete;
    
    protected:

    // A description of the types of resources to be accessed by the pipeline
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    void _createUniformBuffers();

    private:

    VkDescriptorSetLayout _descriptorSetLayout;
    std::vector<VkBuffer> _uniformBuffers;
    std::vector<VkDeviceMemory> _uniformBuffersMemory;
    std::vector<void*> _uniformBuffersMapped;
    uint32_t _max_frames_in_flight;

    void _createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
};

#endif