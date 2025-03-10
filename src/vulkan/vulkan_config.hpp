#pragma once
/* ************************* *
 * Vulkan configuration data *
 * ************************* */
#include "vulkan_structs.hpp"

// Shader file names, locations, and types
const std::vector<ShaderFile> SHADER_FILES = {
    {"src/shaders/fragment/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT},
    {"src/shaders/vertex/vert.spv",   VK_SHADER_STAGE_VERTEX_BIT}
};

// Dynamic states for pipeline creation
#define EN_DYNAMIC_VIEWPORT 0b0001
#define EN_DYNAMIC_SCISSOR  0b0010
// We want this to be dynamic. Actual viewports and scissor rectangles will be set up at draw time
const DynamicStateList DYNAMIC_STATES = {
    EN_DYNAMIC_VIEWPORT | EN_DYNAMIC_SCISSOR,
    {
        VK_DYNAMIC_STATE_VIEWPORT,  // No performance penalty
        VK_DYNAMIC_STATE_SCISSOR    // No performance penalty
    }
};
