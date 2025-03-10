#include "VertexInput.hpp"
/**
 * @file vulkan/pipeline/info/VertexInput.cpp
 * @author Ray Richter
 * @brief Vertex input definitions for pipeline creation
 */

/// @brief Constructs a VkPipelineVertexInputStateCreateInfo with vertex and attribute binding descriptions
VertexInput::VertexInput() {
    // DEBUG_MSG("Called a VertexInput struct constructor");
    createInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    createInfo_.pNext = nullptr; createInfo_.flags = 0;

    createInfo_.vertexBindingDescriptionCount    = static_cast<uint32_t>(vertexBindingDescriptions_.size());
    createInfo_.pVertexBindingDescriptions       = vertexBindingDescriptions_.data();
    createInfo_.vertexAttributeDescriptionCount  = static_cast<uint32_t>(attributeDescriptions_.size());
    createInfo_.pVertexAttributeDescriptions     = attributeDescriptions_.data();
}