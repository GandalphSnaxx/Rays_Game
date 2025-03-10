#include "RasterizerState.hpp"
/**
 * @file vulkan/pipeline/info/RasterizerState.cpp
 * @author Ray Richter
 * @brief Rasterizer state definitions.
 */

RasterizerState::RasterizerState(
const VkPolygonMode polyMode = VK_POLYGON_MODE_FILL, 
const float lineWidth = 1.0f, 
const VkBool32 depthClampEnable = VK_FALSE) {
    // DEBUG_MSG("Called a RasterizerState struct constructor");
    /// TODO: check for lineWidth > 1.0f gpu support
    if (lineWidth > 1.0f) { THROW_ERR("Line width > 1.0 requires a gpu feature!"); }
    /// TODO: Add depth buffer support
    if (depthClampEnable == VK_TRUE) { THROW_ERR("Rasterizer depth support not available! (yet...)"); }

    createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    createInfo_.pNext = nullptr; createInfo_.flags = 0;   // Optional

    createInfo_.depthClampEnable        = depthClampEnable;
    createInfo_.rasterizerDiscardEnable = VK_FALSE;      // Enabling this will disable any output
    createInfo_.polygonMode             = polyMode;
    createInfo_.cullMode                = VK_CULL_MODE_BACK_BIT;        // Set backface culling to support Vulkan cords
    createInfo_.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    createInfo_.depthBiasEnable         = 0.0f;
    createInfo_.depthBiasConstantFactor = 0.0f;
    createInfo_.depthBiasSlopeFactor    = 0.0f;
    createInfo_.lineWidth               = lineWidth;
}

RasterizerState::~RasterizerState() {
    // DEBUG_MSG("Called a RasterizerState struct deconstructor");
}