#include "DescriptorLayoutManager.hpp"
/**
 * @brief Descriptor set layout manager definitions.
 * @author Ray Richter
 */

/// @section Constructors

DescriptorLayoutManager::DescriptorLayoutManager(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("Called DescriptorLayoutManager init constructor");
    VK_CHECK(init_(pDeviceMgr));
}

DescriptorLayoutManager::DescriptorLayoutManager() {
    DEBUG_MSG("Called DescriptorLayoutManager default constructor");
}

DescriptorLayoutManager::~DescriptorLayoutManager() {
    DEBUG_MSG("Called DescriptorLayoutManager deconstructor");
    vkDestroyDescriptorSetLayout(pDeviceMgr_->getDevice(), layout_, nullptr);
}

/// @section Public Member Functions

VkResult DescriptorLayoutManager::init(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("Called DescriptorLayoutManager init function");
    return init_(pDeviceMgr);
}

/// @section Private Member Functions

VkResult DescriptorLayoutManager::init_(DeviceManager *pDeviceMgr) {
    DEBUG_MSG("\tInitalizing DescriptorLayoutManager...");
    VkResult result;
    pDeviceMgr_ = pDeviceMgr;

    VkDescriptorSetLayoutBinding        uboLayoutBinding{};
    // Specify the binding used in the shader and the type of descriptor
    uboLayoutBinding.binding            = 0;
    uboLayoutBinding.descriptorCount    = 1;
    uboLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // Describe which shader stages the descriptor is referenced in
    uboLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    // Only relevant for image sampling related descriptors
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    // Add supporting information for creating a descriptor set layout
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType                    = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount             = 1;
    layoutInfo.pBindings                = &uboLayoutBinding;

    result = vkCreateDescriptorSetLayout(pDeviceMgr_->getDevice(), &layoutInfo, nullptr, &layout_);
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tInit done!");
    return result;
}