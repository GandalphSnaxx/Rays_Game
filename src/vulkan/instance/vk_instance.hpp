#ifndef VK_INSTANCE_HPP__
#define VK_INSTANCE_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/validation_layers/vk_validation_layers.hpp"
#include "raysDebugHelper.hpp"

/// @brief A class for holding, creating, and destroying a Vulkan instance
class VkInstanceClass {
    public:

    // Constructor and deconstructor
    VkInstanceClass();
    ~VkInstanceClass();

    VkResult init();
    VkInstance getInstance();
    inline bool getValidationLayersEnabled() { return validationLayers->enabled(); }
    inline size_t getValidationLayersSize() { return validationLayers->size(); }
    inline const char *const *getValidationLayersData() { return validationLayers->data(); }

    private:

    VkInstance instance;
    VkValidationLayersClass *validationLayers;
};

#endif