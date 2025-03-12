#pragma once
/**
 * @file DescriptorManager.hpp
 * @author Ray Richter
 * @brief Descriptor manager for Vulkan.
 * @version 0.1
 * @date 2025-03-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "vulkan/buffers/UniformBufferManager.hpp"
#include "DescriptorLayoutManager.hpp"
#include "raysDebugHelper.hpp"

/// @brief A class for managing Vulkan descriptors
class DescriptorManager {
public:
    DescriptorManager(UniformBufferManager*, DescriptorLayoutManager*);
    DescriptorManager();
    ~DescriptorManager();
    // Remove the default constructor so it is not accidentally used
    // VkDescriptorClass::VkDescriptorClass() = delete;
    
    VkResult init(UniformBufferManager*, DescriptorLayoutManager*);
    VkResult cleanup();
    inline VkDescriptorSetLayout getLayout() const { return pLayoutMgr_->getLayout(); }
    inline VkDescriptorSet  getSet    (const size_t &i) const { return  sets_[i]; }
    inline VkDescriptorSet* getPSet   (const size_t &i)       { return &sets_[i]; }
    inline VkDescriptorSet  operator[](const size_t &i) const { return  sets_[i]; }

private:
    UniformBufferManager*           pUBOMgr_;
    // VkDescriptorSetLayout           setLayout_;
    DescriptorLayoutManager*        pLayoutMgr_;
    VkDescriptorPool                pool_;
    std::vector<VkDescriptorSet>    sets_;

    VkResult init_(UniformBufferManager*, DescriptorLayoutManager*);
    // VkResult createSetLayout_();
    VkResult createPool_();
    VkResult createSets_();
};