#include "DynamicState.hpp"
/**
 * @file vulkan/pipeline/info/DynamicState.cpp
 * @author Ray Richter
 * @brief Dynamic state and viewport initalization for pipeline creation.
 */

DynamicStateStage::DynamicStateStage(const std::vector<VkDynamicState> &dynamicStates) {
    // DEBUG_MSG("\tCalled a DynamicStateStage struct constructor");

    dynamicCreateInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicCreateInfo_.pNext = nullptr; dynamicCreateInfo_.flags = 0;   // Optional
    dynamicCreateInfo_.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicCreateInfo_.pDynamicStates = dynamicStates.data();

    viewportCreateInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportCreateInfo_.pNext = nullptr;
    viewportCreateInfo_.flags = 0;
    for (const auto &dynamicState : dynamicStates) {
        switch (dynamicState) {
            case VK_DYNAMIC_STATE_SCISSOR:
            viewportCreateInfo_.scissorCount = 1;
            viewportCreateInfo_.pScissors = nullptr;
            break;
            case VK_DYNAMIC_STATE_VIEWPORT:
            viewportCreateInfo_.viewportCount = 1;
            viewportCreateInfo_.pViewports = nullptr;
            break;
            default:
            /// TODO: Add support for more dynamic states
            std::cerr << "DynamicStateStage ERROR: This dynamic state is not supported yet: " << dynamicState << std::endl;
        }
    }
}

DynamicStateStage::~DynamicStateStage() {
    // DEBUG_MSG("\tCalled DynamicState deconstructor");
}