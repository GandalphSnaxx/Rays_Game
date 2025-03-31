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

    std::vector<glm::vec3>  vertices;
    std::vector<uint32_t>   indices;
};

#endif