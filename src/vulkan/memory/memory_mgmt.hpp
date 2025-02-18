#ifndef VULKAN_MEMORY_MANAGEMENT_HPP__
#define VULKAN_MEMORY_MANAGEMENT_HPP__

#include "overhead.hpp"
#include <vector>

/// @brief A class containing variables and functions for managing memory. Child class of VkOverheadClass
class VkMemoryClass : virtual public VkOverheadClass {
    public:
    VkMemoryClass();
    ~VkMemoryClass();
    // Delete the empty constructor so it is not accidentally used
    // VkMemoryClass::VkMemoryClass() = delete;

    protected:

    void _createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void _copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    private:

};

#include <chrono>

/// @brief A class containing functions and variables for managing uniform buffer object related data before the creation of a VkUBOClass. Child class of VkMemoryClass.
class VkUBOOverheadClass : virtual public VkMemoryClass {
    public:
    VkUBOOverheadClass();
    ~VkUBOOverheadClass();

    protected:
    VkDescriptorSetLayout _descriptorSetLayout;

    private:
    VkResult _createDescriptorSetLayout();

};

/// @brief A class containing functions and variables for managing uniform buffer objects. Child class of VkUBOOverheadClass.
class VkUBOClass : virtual public VkUBOOverheadClass {
    public:

    // Constructor and deconstructor
    VkUBOClass(uint32_t max_frames_in_flight);
    ~VkUBOClass();
    // Delete the empty constructor so it is not accidentally used
    VkUBOClass::VkUBOClass() = delete;
    
    void updateUniformBuffer(uint32_t currentImage, VkExtent2D *currentExtent);

    protected:

    // A description of the types of resources to be accessed by the pipeline
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    private:

    std::vector<VkBuffer> _uniformBuffers;
    std::vector<VkDeviceMemory> _uniformBuffersMemory;
    std::vector<void*> _uniformBuffersMapped;
    uint32_t _max_frames_in_flight;
};

#endif