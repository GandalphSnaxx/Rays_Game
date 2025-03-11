#pragma once
/**
 * @file vulkan/pipeline/PipelineManager.hpp
 * @author Ray Richter
 * @brief PipelineManager class declaration
 */
#include "vulkan/render_pass/RenderPass.hpp"
#include "vulkan/descriptors/DescriptorLayoutManager.hpp"
#include "hello_triangle_config.hpp"
#include "raysDebugHelper.hpp"
#include "info/ColorBlendState.hpp"
#include "info/DynamicState.hpp"
#include "info/InputAssembly.hpp"
#include "info/Layout.hpp"
#include "info/MultisamplingState.hpp"
#include "info/RasterizerState.hpp"
#include "info/ShaderFile.hpp"
#include "info/ShaderStage.hpp"
#include "info/VertexInput.hpp"

// Shader file names, entry points, and types
const std::vector<ShaderFile> SHADER_FILES = {
    ShaderFile("src/shaders/fragment/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT),
    ShaderFile("src/shaders/vertex/vert.spv",   VK_SHADER_STAGE_VERTEX_BIT  )
};

/// @brief Graphics pipeline manager for Vulkan.
class PipelineManager {
public:
    /// @brief Constructor initalizer.
    /// @param renderPassManager The render pass to add a pipeline to.
    /// @param descriptorLayout The descriptor layout to add to the pipeline.
    PipelineManager (RenderPassManager*, DescriptorLayoutManager*);
    PipelineManager ();
    ~PipelineManager();

    /// @brief External initalizer function.
    /// @param renderPassManager The render pass to add a pipeline to.
    /// @param descriptorLayout The descriptor layout to add to the pipeline.
    /// @return `VkResult`
    VkResult init(RenderPassManager*, DescriptorLayoutManager*);
    VkResult cleanup();

    VkPipeline          getPipeline() const { return graphicsPipeline_; }
    VkPipelineLayout    getLayout  () const { return pipelineLayout_;   }

private:
    // VkDevice            device_; // Device copy for deconstruction
    VkPipeline                  graphicsPipeline_;
    VkPipelineLayout            pipelineLayout_;
    RenderPassManager*          pRdPassMgr_;
    DescriptorLayoutManager*    pDescLayoutMgr_;

    VkResult init_(RenderPassManager*, DescriptorLayoutManager*);

    VkShaderModule createShaderModule_(const VkDevice& device, const std::vector<char>& code);
};