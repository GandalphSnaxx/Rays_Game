#include "vk_descriptors.hpp"

VkDescriptorClass::VkDescriptorClass() {}

VkDescriptorClass::~VkDescriptorClass() {
    vkDestroyDescriptorSetLayout(device->getDevice(), descriptorSetLayout, nullptr);
}

/// @brief Provide details about every descriptor binding used in the shaders for pipeline creation
/// @param pDevice The device to link the descriptors to
/// @return VkCheck
VkResult VkDescriptorClass::init(VkDevicesClass *pDevice) {
    VkResult result;
    // Link the descriptors to a device
    device = pDevice;

    ERROR_RETURN(createLayout());

    return result;
}

/// @brief Creates a descriptor set layout
/// @return VkResult
VkResult VkDescriptorClass::createLayout() {
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

    return vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout);
}