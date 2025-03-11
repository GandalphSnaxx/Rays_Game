#pragma once
/**
 * @brief Descriptor set layout manager.
 * @author Ray Richter
 */
#include "vulkan/devices/DeviceManager.hpp"
#include "raysDebugHelper.hpp"

class DescriptorLayoutManager {
public:
    DescriptorLayoutManager(DeviceManager *pDeviceMgr);
    DescriptorLayoutManager();
    ~DescriptorLayoutManager();

    VkResult init(DeviceManager *pDeviceMgr);
    VkResult cleanup();
    VkDescriptorSetLayout getLayout() const { return layout_; }

private:
    DeviceManager*          pDeviceMgr_;
    VkDescriptorSetLayout   layout_;
    // std::vector<VkDescriptorSetLayout>  layoutList_;

    VkResult init_(DeviceManager *pDeviceMgr);
};