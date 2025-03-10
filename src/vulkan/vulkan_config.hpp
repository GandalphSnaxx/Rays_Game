#pragma once
/* ************************* *
 * Vulkan configuration data *
 * ************************* */

#include <vector>
#include <vulkan/vulkan.h>
#include "vulkan/pipeline/info/ShaderFile.hpp"

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef EN_VALIDATION_LAYERS
const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation" };
constexpr bool VALIDATION_LAYERS_EN = true;
#else
const std::vector<const char*> VALIDATION_LAYERS = {};
constexpr bool VALIDATION_LAYERS_EN = false;
#endif
