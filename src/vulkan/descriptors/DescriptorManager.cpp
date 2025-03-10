#include "DescriptorManager.hpp"

/// @section Constructors

DescriptorManager::DescriptorManager(UniformBufferManager *pUBOMgr, DescriptorLayoutManager *pLayoutMgr) {
    DEBUG_MSG("Called VkDescriptorClass constructor");
    VK_CHECK(init_(pUBOMgr, pLayoutMgr));
}

DescriptorManager::DescriptorManager() {
    DEBUG_MSG("Called VkDescriptorClass default constructor");
}

DescriptorManager::~DescriptorManager() {
    DEBUG_MSG("Called VkDescriptorClass deconstructor");
    // vkDestroyDescriptorSetLayout(pUBOMgr_->getDevice(), setLayout_, nullptr);
    // Descriptor sets get destroyed when the descriptor pool is destroyed
    vkDestroyDescriptorPool(pUBOMgr_->getDevice(), pool_, nullptr);
}

/// @section Public Member Functions

VkResult DescriptorManager::init(UniformBufferManager *pUBOMgr, DescriptorLayoutManager *pLayoutMgr) {
    DEBUG_MSG("Called DescriptorManager external init function");
    return init_(pUBOMgr, pLayoutMgr);
}

/// @section Private Member Functions

VkResult DescriptorManager::init_(UniformBufferManager *pUBOMgr, DescriptorLayoutManager *pLayoutMgr) {
    DEBUG_MSG("\tInitalizing DescriptorManager...");
    pUBOMgr_ = pUBOMgr;
    pLayoutMgr_ = pLayoutMgr;

    VkResult result;
    // result = createSetLayout_();
    // if (result != VK_SUCCESS) return result;
    result = createPool_();
    if (result != VK_SUCCESS) return result;
    result = createSets_();
    if (result != VK_SUCCESS) return result;

    DEBUG_MSG("\tInit done!");
    return result;
}

// VkResult DescriptorManager::createSetLayout_() {
//     DEBUG_MSG("\t\tCreating descriptor set layout");
//     VkDescriptorSetLayoutBinding uboLayoutBinding{};
//     // Specify the binding used in the shader and the type of descriptor
//     uboLayoutBinding.binding = 0;
//     uboLayoutBinding.descriptorCount = 1;
//     uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     // Describe which shader stages the descriptor is referenced in
//     uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//     // Only relevant for image sampling related descriptors
//     uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

//     // Add supporting information for creating a descriptor set layout
//     VkDescriptorSetLayoutCreateInfo layoutInfo{};
//     layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//     layoutInfo.bindingCount = 1;
//     layoutInfo.pBindings = &uboLayoutBinding;

//     return vkCreateDescriptorSetLayout(pUBOMgr_->getDevice(), &layoutInfo, nullptr, &setLayout_);
// }

VkResult DescriptorManager::createPool_() {
    DEBUG_MSG("\t\tCreating descriptor pool");
    VkDescriptorPoolSize poolSize{};
    // Describe which descriptor types our descriptor sets are going to contain and how many of them
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(pUBOMgr_->getMaxFIF());

    // Structure the poolSize info
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    // Maximum number of poolSets
    poolInfo.maxSets = static_cast<uint32_t>(pUBOMgr_->getMaxFIF());

    return vkCreateDescriptorPool(pUBOMgr_->getDevice(), &poolInfo, nullptr, &pool_);
}

VkResult DescriptorManager::createSets_() {
    DEBUG_MSG("\t\tCreating descriptor sets");
    VkResult result;

    // Set the pool, number of descriptors, and layout
    std::vector<VkDescriptorSetLayout> layouts(pUBOMgr_->getMaxFIF(), pLayoutMgr_->getLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool_;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(pUBOMgr_->getMaxFIF());
    allocInfo.pSetLayouts = layouts.data();

    // Allocate descriptor set handles
    sets_.resize(pUBOMgr_->getMaxFIF());
    VK_CHECK(vkAllocateDescriptorSets(pUBOMgr_->getDevice(), &allocInfo, sets_.data()));

    // Populate every descriptor
    for (size_t i = 0; i < pUBOMgr_->getMaxFIF(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = pUBOMgr_->getBuffer(i);
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        // The set to update
        descriptorWrite.dstSet = sets_[i];
        // No binding and no array
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        // Specify type and size
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        // An array of descriptorCount structs that actually configure the descriptors
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(pUBOMgr_->getDevice(), 1, &descriptorWrite, 0, nullptr);
    }
    return result;
}
