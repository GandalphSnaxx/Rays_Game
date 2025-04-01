/**
 * @file materials.hpp
 * @author Ray Richter
 * @brief Material structs
 * @version 0.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "vk_types.hpp"
#include "constants.hpp"

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static std::array<VkVertexInputBindingDescription, 1>   getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

const std::vector<Vertex> TRIANGLE_VERTICES = {
    // pos          | color
    {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
};

struct Background {
    VkPipeline              pipeline;
    VkPipelineLayout        layout;

    VkDescriptorPool        descriptorPool;
    VkDescriptorSetLayout   descriptorSetLayout;
    VkDescriptorSet         descriptorSet;
};

// enum class MaterialPass : uint8_t {
//     MainColor,
//     Transparent,
//     other
// };

struct MaterialPipeline {
    VkPipeline          pipeline;
    VkPipelineLayout    layout;

    // Return_t init_pipeline(const std::multimap<std::string, VkShaderStageFlagBits>& shaderFiles);
};

// struct MaterialInstance {
//     MaterialPipeline*   pPipeline;
//     VkDescriptorSet     materialSet;
//     MaterialPass        passType;
// };

struct Material {
    MaterialPipeline opaquePipeline;
    MaterialPipeline transparentPipeline;

    VkDescriptorSetLayout materialLayout;
};

struct Object {
    MaterialPipeline    material;
    Buffer              vertexBuffer;
    Buffer              indexBuffer;

    std::vector<Vertex>     vertices;
    std::vector<uint32_t>   indices;

    Return_t draw(VkCommandBuffer& cmd, const VkRenderPass& renderPass, const VkFramebuffer& fb, const VkExtent2D& extent);
};

#endif