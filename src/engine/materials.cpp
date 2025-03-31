/**
 * @file materials.cpp
 * @author Ray Richter
 * @brief Material function definitions
 * @version 0.1
 * @date 2025-03-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "materials.hpp"

// VkVertexInputBindingDescription Vertex::getBindingDescription() {
//     VkVertexInputBindingDescription bindingDescription = {};

//     VkVertexInputBindingDescription bindingDescription{};
//     bindingDescription.binding = 0;
//     bindingDescription.stride = sizeof(Vertex);
//     bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

//     return bindingDescription;
// }

// std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
//     std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

//     attributeDescriptions[0].binding = 0;
//     attributeDescriptions[0].location = 0;
//     attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//     attributeDescriptions[0].offset = offsetof(Vertex, pos);

//     attributeDescriptions[1].binding = 0;
//     attributeDescriptions[1].location = 1;
//     attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//     attributeDescriptions[1].offset = offsetof(Vertex, color);

//     return attributeDescriptions;
// }