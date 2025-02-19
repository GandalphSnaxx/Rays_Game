#include "graphics_pipeline.hpp"

/// @brief Configure pipeline information for pipeline creation
/// @param shader_files A list of shader files to be added to the pipeline
VkPipelineOverheadClass::VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files) {
    DEBUG_MSG("Called VkPipelineOverheadClass constructor");
    _initShaderStages(shader_files);
    _initVertexInput();
    _initInputAssembly(INPUT_ASSEMBLY_TRIANGLE_LIST);
    _initViewportState();
    _initRasterizer(RASTERIZER_FILL);
    _initMultisampling(MULTISAMPLING_DISABLE);
    _initColorBlendAttachment(COLOR_BLEND_ATTACHMENT_NO_BLEND);

    _initPipelineInfo();
}

VkPipelineOverheadClass::~VkPipelineOverheadClass() {
    DEBUG_MSG("Called VkPipelineOverheadClass deconstructor");
    _shaderStages.~vector();
}

/// @brief Initalizes shader stage information for each shader file passed in
/// @param shader_files A list of shader files to initalize
void VkPipelineOverheadClass::_initShaderStages(const std::vector<ShaderFile> &shader_files) {
    uint32_t numStages = shader_files.size();
    // Set the size of the shader stages list
    _shaderStages.resize(numStages);

    // Iterate through the shader files and add initalize them in the shader stages list
    for (uint32_t i = 0; i < numStages; i++) {
        // Get the binary for this shader module
        auto shader_binary = _readFile(shader_files[i].fileName);

        // Create a shader module from the binary
        VkShaderModule shaderModule = _createShaderModule(shader_binary);

        // Configure the current shader stage struct
        _shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        _shaderStages[i].pNext = nullptr; // Optional
        _shaderStages[i].flags = 0; // Optional
        _shaderStages[i].stage = shader_files[i].shaderType;
        _shaderStages[i].module = shaderModule;
        _shaderStages[i].pName = "main";
        _shaderStages[i].pSpecializationInfo = nullptr; // Optional
    }
}

/// @brief Initalizes vertex input data with binding descriptions and attribute descriptions
void VkPipelineOverheadClass::_initVertexInput() {
    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.pNext = nullptr;
    _vertexInputInfo.flags = 0;
    _vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_vertexBindingDescriptions.size());
    _vertexInputInfo.pVertexBindingDescriptions = _vertexBindingDescriptions.data();
    _vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_attributeDescriptions.size());
    _vertexInputInfo.pVertexAttributeDescriptions = _attributeDescriptions.data();
}

/// @brief Initalizes the input assembly type for pipeline creation
void VkPipelineOverheadClass::_initInputAssembly(const VkPipelineInputAssemblyStateCreateInfo inputAssemblyType) {
    _inputAssembly = inputAssemblyType;
}

/// @brief Initalizes scissor and viewport states for pipeline creation
void VkPipelineOverheadClass::_initViewportState() {
    _viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportState.pNext = nullptr;
    _viewportState.flags = 0;
    if (DYNAMIC_STATES.enabledStateBytefield & EN_DYNAMIC_SCISSOR) {
        // Set scissors to be dynamic
        _viewportState.scissorCount = 1;
        _viewportState.pScissors = nullptr;
    } if (DYNAMIC_STATES.enabledStateBytefield & EN_DYNAMIC_VIEWPORT) {
        // Set viewport to be dynamic
        _viewportState.viewportCount = 1;
        _viewportState.pViewports = nullptr;
    } else {
        /// TODO: Configure non-dynamic viewport state
        _viewportState.viewportCount = 1;
        _viewportState.pViewports = nullptr;
        _viewportState.scissorCount = 1;
        _viewportState.pScissors = nullptr;
    }
}

/// @brief The rasterizer takes geometry shaped by the verticies from the vertex shader and turns it into fragments to be shaded by the fragment shader
void VkPipelineOverheadClass::_initRasterizer(const VkPipelineRasterizationStateCreateInfo rasterizerType) {
    /// TODO: Check gpu features for certain rasterizer modes
    _rasterizer = rasterizerType;
}

/// @brief Initalizes multisampling configuration. One of the ways to perform anti-aliasing. Enabling requires a gpu feature. Disabled for now
void VkPipelineOverheadClass::_initMultisampling(const VkPipelineMultisampleStateCreateInfo multisamplingConfig) {
    /// TODO: Check gpu features to enable certain multisampling modes
    _multisampling = multisamplingConfig;
}

/// @brief Mix or combine color with the fragment shader output
void VkPipelineOverheadClass::_initColorBlendAttachment(const VkPipelineColorBlendAttachmentState colorBlendAttachmentConfig) {
    _colorBlendAttachment = colorBlendAttachmentConfig;
}

/// @brief References the array of structures for all framebuffers and allows setting blend constants
void VkPipelineOverheadClass::_initColorBlend(const VkPipelineColorBlendStateCreateInfo colorBlendConfig) {
    _colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    _colorBlending.logicOpEnable = VK_FALSE;
    _colorBlending.logicOp = VK_LOGIC_OP_COPY;
    _colorBlending.attachmentCount = 1;
    _colorBlending.pAttachments = &_colorBlendAttachment;
    _colorBlending.blendConstants[0] = 0.0f;
    _colorBlending.blendConstants[1] = 0.0f;
    _colorBlending.blendConstants[2] = 0.0f;
    _colorBlending.blendConstants[3] = 0.0f;
}

/// @brief Initalizes a VkGraphicsPipelineCreateInfo struct
void VkPipelineOverheadClass::_initPipelineInfo() {
    // Reference the array of VkPipelineShaderStageCreateInfo structs
    _pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // Set the descriptors the shaders will be using
    _pipelineLayoutInfo.setLayoutCount = 1;
    _pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;
    // Set the shader stages
    _pipelineInfo.stageCount = 2;
    _pipelineInfo.pStages = &_shaderStages.data();
    // Reference the structures described in the fixed-function stage
    _pipelineInfo.pVertexInputState = &_vertexInputInfo;
    _pipelineInfo.pInputAssemblyState = &_inputAssembly;
    _pipelineInfo.pViewportState = &_viewportState;
    _pipelineInfo.pRasterizationState = &_rasterizer;
    _pipelineInfo.pMultisampleState = &_multisampling;
    _pipelineInfo.pDepthStencilState = nullptr; // Optional
    _pipelineInfo.pColorBlendState = &_colorBlending;
    _pipelineInfo.pDynamicState = &_dynamicState;
    // Reference the pipeline layout
    _pipelineInfo.layout = _pipelineLayout;
    // Reference the renderPass
    _pipelineInfo.renderPass = _renderPass;
    _pipelineInfo.subpass = 0;
    // Optional: create a pipeline by deriving from an existing pipeline
    _pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    _pipelineInfo.basePipelineIndex = -1; // Optional
}