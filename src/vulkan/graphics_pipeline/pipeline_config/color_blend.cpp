#include "structs.hpp"

/// @brief Initalizes color blending configuration for pipeline creation
/// @param blendModes A list of VkPipelineColorBlendAttachmentStates. There should be one per framebuffer. List of available constants:
///
/// - COLOR_BLEND_ATTACHMENT_NULL - Disable any output
///
/// - COLOR_BLEND_ATTACHMENT_NO_BLEND - No Blending
///
/// - COLOR_BLEND_ATTACHMENT_ALPHA_BLEND - Standard alpha blend
///
/// . @param logicOp A global logical operation
ColorBlendState::ColorBlendState(const std::vector<VkPipelineColorBlendAttachmentState> *blendModes, const VkLogicOp logicOp) {
    DEBUG_MSG("Called a ColorBlendState struct constructor");
    // Get the number of color blend attachments
    uint32_t numAttachments = blendModes->size();
    /// TODO: Make sure there is one blendMode for each framebuffer
    if (numAttachments < 1 && logicOp == 0) { THROW_ERR("Invalid number of Color Blend Attachments!"); }

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    createInfo.pNext = nullptr; createInfo.flags = 0;   // Optional
    createInfo.logicOpEnable = (logicOp != VK_LOGIC_OP_CLEAR) ? VK_TRUE : VK_FALSE;
    createInfo.logicOp = logicOp;
    createInfo.attachmentCount = numAttachments;
    createInfo.pAttachments = blendModes->data();
    createInfo.blendConstants[0] = _blendConstants[0];
    createInfo.blendConstants[1] = _blendConstants[1];
    createInfo.blendConstants[2] = _blendConstants[2];
    createInfo.blendConstants[3] = _blendConstants[3];
}

/// @brief ColorBlendState deconstructor
ColorBlendState::~ColorBlendState() {
    DEBUG_MSG("Called a ColorBlendState struct deconstructor");
}