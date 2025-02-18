#include "graphics_pipeline.hpp"

VkPipelineOverheadClass::VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files) {
    DEBUG_MSG("Called VkPipelineOverheadClass constructor");
    _initShaderStages(shader_files);
    _initVertexInput();

    _initPipelineInfo();
}

VkPipelineOverheadClass::~VkPipelineOverheadClass() {
    DEBUG_MSG("Called VkPipelineOverheadClass deconstructor");
    _shaderStages.~vector();
}


/// @brief Read a binary file and return the raw binary
/// @param filename The file to be read
/// @return The file's raw binary
std::vector<char> VkPipelineOverheadClass::_readFile(const std::string& filename) {
    // Open a file at the end so we can get its size
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    // Make sure the file opened correctly
    if (!file.is_open()) {
        THROW_ERR("failed to open file!");
    }

    // Get file size
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    // Goto the beginning of the file and read all of the bytes at once
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    // Check if we are reading the files correctly
    DEBUG_VAR(fileSize);

    // Close the file and return the read data
    file.close();
    return buffer;
}

/// @brief Adds raw shader bytecode to a shader module
/// @param code A pointer to the raw bytecode to be passed into the shader module
/// @return VkShaderModule
VkShaderModule VkPipelineOverheadClass::_createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    // Set the configuration for the shader module
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    // Create the shader module
    VkShaderModule shaderModule;
    VK_CHECK(vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule));
    return shaderModule;
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

/// @brief Initalizes vertex input info struct
void VkPipelineOverheadClass::_initVertexInput() {
    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    _vertexInputInfo.vertexBindingDescriptionCount = 1;
    _vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    _vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    _vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}

/// @brief Initalizes the struct describing what kind of geometry will be drawn and should primitive restart be enabled
void VkPipelineOverheadClass::_initInputAssembly() {
    _inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    _inputAssembly.primitiveRestartEnable = VK_FALSE;
}

/// @brief Describes what region of the framebuffer the output will be rendered to. Typically (0, 0) to (width, height). 
///     We want this to be dynamic. Actual viewports and scissor rectangles will be set up at draw time
void VkPipelineOverheadClass::_initViewportState() {
    _viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    _viewportState.viewportCount = 1;
    _viewportState.scissorCount = 1;
}

/// @brief The rasterizer takes geometry shaped by the verticies from the vertex shader and turns it into fragments to be shaded by the fragment shader
void VkPipelineOverheadClass::_initRasterizer() {
    _rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizer.depthClampEnable = VK_FALSE;
    _rasterizer.rasterizerDiscardEnable = VK_FALSE;
    _rasterizer.polygonMode = VK_POLYGON_MODE_FILL;  // Can also be edges, or points. May require a gpu feature
    _rasterizer.lineWidth = 1.0f;
    // Set the backface culling to work with Vulkan cords
    _rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    _rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    _rasterizer.depthBiasEnable = VK_FALSE;
    _rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    _rasterizer.depthBiasClamp = 0.0f; // Optional
    _rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
}

/// @brief Initalizes multisampling configuration. One of the ways to perform anti-aliasing. Enabling requires a gpu feature. Disabled for now
void VkPipelineOverheadClass::_initMultisampling() {
    _multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisampling.sampleShadingEnable = VK_FALSE;
    _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    _multisampling.minSampleShading = 1.0f; // Optional
    _multisampling.pSampleMask = nullptr; // Optional
    _multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    _multisampling.alphaToOneEnable = VK_FALSE; // Optional
}

/// @brief Mix or combine color with the fragment shader output
void VkPipelineOverheadClass::_initColorBlendAttachment() {
    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _colorBlendAttachment.blendEnable = VK_FALSE;
    _colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    _colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    _colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    _colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    _colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    _colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
}

/// @brief References the array of structures for all framebuffers and allows setting blend constants
void VkPipelineOverheadClass::_initColorBlend() {
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