#include "graphics_pipeline.hpp"

/// @brief Configure pipeline information for pipeline creation
/// @param shader_files A list of shader files to be added to the pipeline
VkPipelineOverheadClass::VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files) :
    // Parent classes and private struct initalizers called in ascending order
    _shaderStages(shader_files, _device),
    _vertexInputs(),
    _inputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE),
    _viewportState(&DYNAMIC_STATES),
    _rasterizer(VK_POLYGON_MODE_FILL),
    _multisampling(VK_SAMPLE_COUNT_1_BIT),
    _colorBlending(&const std::vector<VkPipelineColorBlendAttachmentState>{COLOR_BLEND_ATTACHMENT_NO_BLEND}, VK_LOGIC_OP_CLEAR),
    _dynamicStates(&DYNAMIC_STATES),
    _pipelineLayoutInfo(&const std::vector<VkDescriptorSetLayout>{_descriptorSetLayout}),
    VkOverheadClass()
{
    DEBUG_MSG("Called VkPipelineOverheadClass constructor");

    // Initalize _pipelineLayout in reference to a device and LayoutCreateInfo
    VK_CHECK(vkCreatePipelineLayout(_device, &_pipelineLayoutInfo.createInfo, nullptr, &_pipelineLayout));

    // Initalize _pipelineInfo by adding all createInfos
    
    // Reference the array of VkPipelineShaderStageCreateInfo structs
    _pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // Set the shader stages
    _pipelineInfo.stageCount = _shaderStages.createInfo.size();
    _pipelineInfo.pStages = _shaderStages.createInfo.data();
    // Reference the structures described in the fixed-function stage
    _pipelineInfo.pVertexInputState = &_vertexInputs.createInfo;
    _pipelineInfo.pInputAssemblyState = &_inputAssembly.createInfo;
    _pipelineInfo.pViewportState = &_viewportState.createInfo;
    _pipelineInfo.pRasterizationState = &_rasterizer.createInfo;
    _pipelineInfo.pMultisampleState = &_multisampling.createInfo;
    _pipelineInfo.pDepthStencilState = nullptr; // Optional
    _pipelineInfo.pColorBlendState = &_colorBlending.createInfo;
    _pipelineInfo.pDynamicState = &_dynamicState.createInfo;
    // Reference the pipeline layout
    _pipelineInfo.layout = _pipelineLayout;
    // Reference the renderPass
    _pipelineInfo.renderPass = _renderPass;
    _pipelineInfo.subpass = 0;
    // Optional: create a pipeline by deriving from an existing pipeline
    _pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    _pipelineInfo.basePipelineIndex = -1; // Optional
}

VkPipelineOverheadClass::~VkPipelineOverheadClass() {
    DEBUG_MSG("Called VkPipelineOverheadClass deconstructor");
    // _shaderStages.~vector();
}