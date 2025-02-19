#include "structs.hpp"

/// @brief Constructs a VkPipelineVertexInputStateCreateInfo with vertex and attribute binding descriptions
VertexInput::VertexInput() {
    DEBUG_MSG("Called a VertexInput struct constructor");
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBindingDescriptions.size());
    createInfo.pVertexBindingDescriptions = vertexBindingDescriptions.data();
    createInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    createInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}

/// @brief Cleans up any data in this struct
VertexInput::~VertexInput() {
    DEBUG_MSG("Called a VertexInput struct deconstructor");
}