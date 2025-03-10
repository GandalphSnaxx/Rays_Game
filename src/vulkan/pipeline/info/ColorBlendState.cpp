#include "ColorBlendState.hpp"
/**
 * @file
 * @author Ray Richter
 * @brief Color blend state definitions
 */

ColorBlendState::ColorBlendState(
const std::vector<VkPipelineColorBlendAttachmentState> &blendModes, 
const VkLogicOp &logicOp) {

    // Get the number of color blend attachments
    uint32_t numAttachments = blendModes.size();
    /// TODO: Make sure there is one blendMode for each framebuffer
    if (numAttachments < 1 && logicOp == 0) { THROW_ERR("ColorBlendState ERROR: Invalid number of Color Blend Attachments!"); }

    createInfo_.sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    createInfo_.pNext              = nullptr; 
    createInfo_.flags              = 0;   // Optional
    createInfo_.logicOpEnable      = (logicOp != VK_LOGIC_OP_CLEAR) ? VK_TRUE : VK_FALSE;
    createInfo_.logicOp            = logicOp;
    createInfo_.attachmentCount    = numAttachments;
    createInfo_.pAttachments       = blendModes.data();
    createInfo_.blendConstants[0]  = blendConstants_[0];
    createInfo_.blendConstants[1]  = blendConstants_[1];
    createInfo_.blendConstants[2]  = blendConstants_[2];
    createInfo_.blendConstants[3]  = blendConstants_[3];
}
