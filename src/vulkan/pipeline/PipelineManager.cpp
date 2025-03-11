#include "PipelineManager.hpp"
/**
 * @file vulkan/pipeline/PipelineManager.cpp
 * @author Ray Richter
 * @brief PipelineManager class definitions
 */

/// @section Constructors

PipelineManager::PipelineManager(RenderPassManager *pRdPassMgr, DescriptorLayoutManager *pDescLayoutMgr) {
    DEBUG_MSG("Called PipelineManager init constructor");
    VK_CHECK(init_(pRdPassMgr, pDescLayoutMgr));
}

PipelineManager::PipelineManager() {
    DEBUG_MSG("Called PipelineManager default constructor");
}

PipelineManager::~PipelineManager() {
    DEBUG_MSG("Called PipelineManager deconstructor");
    // vkDestroyPipelineLayout(pRdPassMgr_->getDevice(), pipelineLayout_, nullptr);
}

/// @section Public Member Functions

VkResult PipelineManager::init(RenderPassManager *pRdPassMgr, DescriptorLayoutManager *pDescLayoutMgr) {
    DEBUG_MSG("Called PipelineManager external init function");
    return init_(pRdPassMgr, pDescLayoutMgr);
}

VkResult PipelineManager::cleanup() {
    vkDestroyPipeline(pRdPassMgr_->getDevice(), graphicsPipeline_, nullptr);
    vkDestroyPipelineLayout(pRdPassMgr_->getDevice(), pipelineLayout_, nullptr);
    return VK_SUCCESS;
}

/// @section Private Member Functions

VkResult PipelineManager::init_(RenderPassManager *pRdPassMgr, DescriptorLayoutManager *pDescLayoutMgr) {
    DEBUG_MSG("\tInitalizing a gfx pipeline...");
    pRdPassMgr_ = pRdPassMgr;
    pDescLayoutMgr_ = pDescLayoutMgr;
    VkResult result;

    // Get shader binaries
    ShaderStage shaderStages(SHADER_FILES, pRdPassMgr_->getDevice());

    // Load the vertex data using the binding descriptions
    VertexInput vertexInput;

    // This struct describes what kind of geometry will be drawn and should primitive restart be enabled
    InputAssembly inputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    // The rasterizer takes geometry shaped by the verticies from the vertex shader and turns it into fragments to be shaded by 
    // the fragment shader
    RasterizerState rasterizerState(VK_POLYGON_MODE_FILL);

    // One of the ways to perform anti-aliasing. Enabling requires a gpu feature. Disabled for now
    MultisamplingState multisamplingState(VK_SAMPLE_COUNT_1_BIT);

    // If using depth and/or stencil buffer, configure here using VkPipelineDepthStencilCreateInfo()

    // References the array of structures for all framebuffers and allows setting blend constants
    ColorBlendState colorBlending({COLOR_BLEND_ATTACHMENT_NO_BLEND}, VK_LOGIC_OP_COPY);

    // Allow these values to be set at draw time
    // Describes what region of the framebuffer the output will be rendered to. Typically (0, 0) to (width, height)
    // We want this to be dynamic. Actual viewports and scissor rectangles will be set up at draw time
    DynamicStateStage dynamicStates(DYNAMIC_STATES);

    // Create pipeline layout configuration
    Layout pipelineLayout(pRdPassMgr_->getDevice(), &pipelineLayout_, {pDescLayoutMgr_->getLayout()});

    // Start creating the pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    // Reference the array of VkPipelineShaderStageCreateInfo structs
    pipelineInfo.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // Set the shader stages
    pipelineInfo.stageCount             = shaderStages.size();
    pipelineInfo.pStages                = shaderStages.data();
    // Reference the structures described in the fixed-function stage
    pipelineInfo.pVertexInputState      = vertexInput.data();
    pipelineInfo.pInputAssemblyState    = inputAssembly.data();
    pipelineInfo.pViewportState         = dynamicStates.viewportData();
    pipelineInfo.pRasterizationState    = rasterizerState.data();
    pipelineInfo.pMultisampleState      = multisamplingState.data();
    pipelineInfo.pDepthStencilState     = nullptr; // Optional
    pipelineInfo.pColorBlendState       = colorBlending.data();
    pipelineInfo.pDynamicState          = dynamicStates.dynamicData();
    // Reference the pipeline layout
    pipelineInfo.layout                 = pipelineLayout_;
    // Reference the renderPass
    pipelineInfo.renderPass             = pRdPassMgr_->getRenderPass();
    pipelineInfo.subpass                = 0;
    // Optional: create a pipeline by deriving from an existing pipeline
    pipelineInfo.basePipelineHandle     = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex      = -1; // Optional

    result = vkCreateGraphicsPipelines(pRdPassMgr_->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_);
    if (result != VK_SUCCESS) return result;
    
    // Cleanup. This gets called by ~ShaderStage()
    // vkDestroyShaderModule(device, fragShaderModule, nullptr);
    // vkDestroyShaderModule(device, vertShaderModule, nullptr);
    DEBUG_MSG("\tDone!");
    return result;
}