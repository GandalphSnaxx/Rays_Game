#include "memory_mgmt.hpp"

/* ************************** *
 * VkUBOOverheadClass section * 
 * ************************** */

/// @brief VkUBOOverheadClass constructor. Creates a descriptor set layout for pipeline creation
VkUBOOverheadClass::VkUBOOverheadClass() {
    DEBUG_MSG("Called VkUBOOverheadClass constructor");
    VK_CHECK(_createDescriptorSetLayout());
}

/// @brief Provides details about every descriptor binding used in the shaders for pipeline creation
/// @return VkResult
VkResult VkUBOOverheadClass::_createDescriptorSetLayout() {
    DEBUG_MSG("\tCreating descriptor set layout...");

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

    return vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout);
}

/* ************************** *
 *     VkUBOClass section     * 
 * ************************** */

/// @brief Allocates memory space for uniform buffer objects
/// @param max_frames_in_flight The maximum number of frames in flight
VkUBOClass::VkUBOClass(uint32_t max_frames_in_flight) {
    DEBUG_MSG("Called VkUBOClass constructor");
    
    _max_frames_in_flight = max_frames_in_flight;

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

/// @brief Clears memory space used by any uniform buffer objects
VkUBOClass::~VkUBOClass() {
    DEBUG_MSG("Called VkUBOClass deconstructor");

}

/// @brief Update object verticies to create a 90*/s rotation
/// @param currentImage The image to be updated
/// @param currentExtent The current swapchain extent
void VkUBOClass::updateUniformBuffer(uint32_t currentImage, VkExtent2D *currentExtent) {
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
        currentExtent->width / (float) currentExtent->height, 
        0.1f, 10.0f);

    // Convert from OpenGL cords to Vulkan cords (Flip Y axis)
    ubo.proj[1][1] *= -1;

    // Copy data from the object to the current uniform buffer
    memcpy(_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
