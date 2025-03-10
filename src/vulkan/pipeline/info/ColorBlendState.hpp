#pragma once
/**
 * @file vulkan/pipeline/info/ColorBlendState.hpp
 * @author Ray Richter
 * @brief Color blend state configurator for pipeline creation.
 */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include "raysDebugHelper.hpp"

// Disable any output
constexpr VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_NULL = {
    VK_FALSE,                   // blendEnable
    VK_BLEND_FACTOR_ZERO,       // srcColorBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstColorBlendFactor
    VK_BLEND_OP_ADD,            // colorBlendOp
    VK_BLEND_FACTOR_ZERO,       // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,            // alphaBlendOp
    0                           // colorWriteMask bitfield
};
// Disable color blending attachments but keep the output
constexpr VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_NO_BLEND = {
    VK_FALSE,                   // blendEnable
    VK_BLEND_FACTOR_ONE,        // srcColorBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstColorBlendFactor
    VK_BLEND_OP_ADD,            // colorBlendOp
    VK_BLEND_FACTOR_ONE,        // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,            // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |  // colorWriteMask bitfield
    VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | 
    VK_COLOR_COMPONENT_A_BIT
};
// Enable standard alpha blending
constexpr VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_ALPHA_BLEND = {
    VK_TRUE,                                // blendEnable
    VK_BLEND_FACTOR_SRC_ALPHA,              // srcColorBlendFactor
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,    // dstColorBlendFactor
    VK_BLEND_OP_ADD,                        // colorBlendOp
    VK_BLEND_FACTOR_ONE,                    // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                   // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                        // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |              // colorWriteMask bitfield
    VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | 
    VK_COLOR_COMPONENT_A_BIT
};

/// @brief A struct holding VkPipelineColorBlendStateCreateInfo related info for pipeline creation
struct ColorBlendState {
public:
    ColorBlendState(
        const std::vector<VkPipelineColorBlendAttachmentState> &blendModes = {COLOR_BLEND_ATTACHMENT_NO_BLEND}, 
        const VkLogicOp &logicOp = VK_LOGIC_OP_CLEAR);
    ~ColorBlendState() {}
    // Delete default constructor so it is not accidentally used
    ColorBlendState::ColorBlendState() = delete;

    inline const VkPipelineColorBlendStateCreateInfo* data() const noexcept { return &createInfo_; }

private:
    VkPipelineColorBlendStateCreateInfo createInfo_{};

    std::array<float, 4> blendConstants_ = {0.0f, 0.0f, 0.0f, 0.0f};
    // std::vector<VkPipelineColorBlendAttachmentState> attachmentStates_{};
};