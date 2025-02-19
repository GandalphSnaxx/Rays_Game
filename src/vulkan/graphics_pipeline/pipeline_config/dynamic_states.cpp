#include "structs.hpp"

DynamicStateStage::DynamicStateStage(const DynamicStateList *dynamicStates) {
    DEBUG_MSG("Called a DynamicStateStage struct constructor");

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    createInfo.pNext = nullptr; createInfo.flags = 0;   // Optional
    createInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates->list.size());
    createInfo.pDynamicStates = dynamicStates->list.data();
}

DynamicStateStage::~DynamicStateStage() {
    DEBUG_MSG("Called a DynamicStateStage struct deconstructor");
}