#pragma once
/**
 * @file vulkan/pipeline/info/VertexInput.hpp
 * @author Ray Richter
 * @brief Vertex input info for pipeline creation
 */
#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan/vertex/Vertex.hpp"

/// @brief A struct for holding VkPipelineVertexInputStateCreateInfo and variables related to it.
struct VertexInput {
public:    
    VertexInput();
    // ~VertexInput();

    inline const VkPipelineVertexInputStateCreateInfo* data() const noexcept { return &createInfo_; }

private:
    VkPipelineVertexInputStateCreateInfo createInfo_{};
    std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions_ = Vertex::getBindingDescription();
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions_   = Vertex::getAttributeDescriptions();
};