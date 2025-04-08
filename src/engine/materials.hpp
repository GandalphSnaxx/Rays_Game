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
    glm::vec3 pos;
    glm::vec3 color;

    static std::array<VkVertexInputBindingDescription, 1>   getBindingDescriptions();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

const std::vector<Vertex> TRIANGLE_VERTICES = {
    // pos              | color
    {{ 0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
};

const std::vector<uint32_t> TRIANGLE_INDICES = {
    0, 1, 2
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

typedef enum PipelineID {
    PIPELINE_INVALID,
    PIPELINE_TRIANGLE,
    PIPELINE_OPAQUE,
    PIPELINE_TRANSPARENT,
    PIPELINE_MAX_ID
} PipelineID;

struct MaterialPipeline {
    VkPipeline          pipeline;
    VkPipelineLayout    layout;
    // PipelineID          id;

    // Return_t init_pipeline(const std::multimap<std::string, VkShaderStageFlagBits>& shaderFiles);
    // std::string name() const { return (std::string)(size_t)id; }
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
    // VkBuffer            vertexBuffer;
    // VkBuffer            indexBuffer;

    std::vector<Vertex>     vertices;
    std::vector<uint32_t>   indices;

    Return_t draw(VkCommandBuffer& cmd, const VkRenderPass& renderPass, const VkFramebuffer& fb, const VkExtent2D& extent);
    Return_t init_buffers();
    Return_t init_pipeline(VkData* vk, RenderData* rd, DeletionQueue* deleteQueue);
    Return_t init_default_data();
};

#endif