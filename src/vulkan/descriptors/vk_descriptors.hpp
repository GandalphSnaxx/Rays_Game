#ifndef VK_DESCRIPTORS_HPP__
#define VK_DESCRIPTORS_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/devices/vk_devices.hpp"
#include "raysDebugHelper.hpp"

class VkDescriptorClass {
    public:

    VkDescriptorClass();
    ~VkDescriptorClass();
    VkResult init(VkDevicesClass *pDevice);
    VkDevicesClass *getDevice() { return device; }
    VkDescriptorSetLayout *getLayout() { return &descriptorSetLayout; }

    private:

    VkDescriptorSetLayout descriptorSetLayout;
    VkDevicesClass *device;

    VkResult createLayout();
};

#endif