#include "structs.hpp"

PipelineLayout::PipelineLayout(const std::vector<VkDescriptorSetLayout> *descriptorSetLayouts) {
    DEBUG_MSG("Called a PipelineLayout struct constructor");
    
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr; createInfo.flags = 0;   // Optional
    createInfo.setLayoutCount = descriptorSetLayouts->size();
    createInfo.pSetLayouts = descriptorSetLayouts->data();
    createInfo.pushConstantRangeCount = 0;
    createInfo.pPushConstantRanges = nullptr;
}

PipelineLayout::~PipelineLayout() {
    DEBUG_MSG("Called a PipelineLayout struct deconstructor");
}