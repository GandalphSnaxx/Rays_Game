#include "descriptors.hpp"

VkDescriptorClass::VkDescriptorClass(const WindowInit *window_init, const char *appName, uint32_t max_frames_in_flight) :
    // Call each parent class in ascending order
    VkDeviceClass(window_init, appName),
    VkSurfaceClass(window_init, appName),
    VkInstanceClass(appName),
    VkWindowClass(window_init) 
{
    DEBUG_MSG("Called VkDescriptorClass constructor");
    _max_frames_in_flight = max_frames_in_flight;
    
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // Specify the binding used in the shader and the type of descriptor
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // Describe which shader stages the descriptor is referenced in
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    // Only relevant for image sampling related descriptors
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    // Add supporting information for creating a descriptor set layout
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    VK_CHECK(vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout));
}

VkDescriptorClass::~VkDescriptorClass() {
    DEBUG_MSG("Called VkDescriptorClass deconstructor");
    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
}

/// @brief Use persistent mapping so we do not have to map the buffer every time we update it.
void VkDescriptorClass::_createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    // Set the buffers to their maximum size
    _uniformBuffers.resize(_max_frames_in_flight);
    _uniformBuffersMemory.resize(_max_frames_in_flight);
    _uniformBuffersMapped.resize(_max_frames_in_flight);

    // Create and map each buffer
    for (size_t i = 0; i < _max_frames_in_flight; i++) {
        _createBuffer(bufferSize, 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            _uniformBuffers[i], 
            _uniformBuffersMemory[i]);

        vkMapMemory(_device, _uniformBuffersMemory[i], 0, bufferSize, 0, &_uniformBuffersMapped[i]);
    }
}

/// @brief Buffers in Vulkan are regions of memory used for storing arbitrary data that can be read by the graphics card
/// @param size The size of the buffer to be created
/// @param usage Buffer usage flag bits
/// @param properties Memory flags in a bitfield
/// @param buffer A pointer to the buffer
/// @param bufferMemory A pointer to the buffer memory
void VkDescriptorClass::_createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
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