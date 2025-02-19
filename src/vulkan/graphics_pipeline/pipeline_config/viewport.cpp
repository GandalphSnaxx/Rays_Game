#include "structs.hpp"

/// @brief Initalizes a VkPipelineViewportStateCreateInfo
/// @param dynamicStates A list of dynamic states to enable
ViewportState::ViewportState(const DynamicStateList dynamicStates) {
    DEBUG_MSG("Called a ViewportState struct constructor");
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    if (dynamicStates.enabledStateBytefield & EN_DYNAMIC_SCISSOR) {
        // Set scissors to be dynamic
        createInfo.scissorCount = 1;
        createInfo.pScissors = nullptr;
    } if (dynamicStates.enabledStateBytefield & EN_DYNAMIC_VIEWPORT) {
        // Set viewport to be dynamic
        createInfo.viewportCount = 1;
        createInfo.pViewports = nullptr;
    } else {
        /// TODO: Configure non-dynamic viewport state
        createInfo.viewportCount = 1;
        createInfo.pViewports = nullptr;
        createInfo.scissorCount = 1;
        createInfo.pScissors = nullptr;
    }
}

/// @brief Deconstructor
ViewportState::~ViewportState() {
    DEBUG_MSG("Called a ViewportState struct deconstructor");
}