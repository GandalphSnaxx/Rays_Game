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
#include <iostream>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define MSG_LOG(m) std::cout << "[MATERIAL] Message in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl
#define ERR_LOG(m) std::cout << "[MATERIAL]  ERROR  in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl

Return_t load_shader(VkData* vk, const std::filesystem::path& path, VkShaderModule* shaderModule) {
    std::string msg = "Loading shader file from: " + path.string();

    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        ERR_LOG(msg +" Fail!\n\tCould not open file");
        return FILE_ERROR;
    }
    // std::filesystem::path path2;
    msg += '.';

    // Get the file size
    stream.seekg(0, std::ios_base::end);
    std::streampos size = stream.tellg();
    stream.seekg(0, std::ios_base::beg);
    msg += '.';

    // Read the file
    std::vector<char> buffer(size);
    if (!stream.read(buffer.data(), size)) {
        ERR_LOG(msg +" Fail!\n\tCould not read shader file: " + path.string());
        return FILE_ERROR;
    }
    msg += '.';

    stream.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = (uint32_t*)buffer.data();
    createInfo.codeSize = buffer.size();
    msg += '.';

    if (vk->dispTable.createShaderModule(&createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        ERR_LOG(msg +" Fail!\n\tFailed to load shader");
        return FILE_ERROR;
    }
    msg += '.';

    MSG_LOG(msg +" Success");
    return SUCCESS;
}

std::array<VkVertexInputBindingDescription, 1> Vertex::getBindingDescriptions() {
    std::array<VkVertexInputBindingDescription, 1> bindingDescriptions = {};
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

Return_t Object::draw(VkCommandBuffer& cmd, const VkRenderPass& renderPass, const VkFramebuffer& fb, const VkExtent2D& extent) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        ERR_LOG("Failed to begin recording command buffer");
        return COMMAND_ERROR;
    }

    VkRenderPassBeginInfo renderPassInfo= {};
    renderPassInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass           = renderPass;
    renderPassInfo.framebuffer          = fb;
    renderPassInfo.renderArea.offset    = { 0, 0 };
    renderPassInfo.renderArea.extent    = extent;

    VkClearValue clearColor         = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
    renderPassInfo.clearValueCount  = 1;
    renderPassInfo.pClearValues     = &clearColor;

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material.pipeline);

        VkViewport viewport = {};
        viewport.x          = 0.0f;
        viewport.y          = 0.0f;
        viewport.width      = (float) extent.width;
        viewport.height     = (float) extent.height;
        viewport.minDepth   = 0.0f;
        viewport.maxDepth   = 1.0f;
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset   = { 0, 0 };
        scissor.extent   = extent;
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        // VkBuffer vertexBuffers[] = { vertexBuffer.handle };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffer.handle, offsets);

        vkCmdBindIndexBuffer(cmd, indexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(cmd, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(cmd);

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        ERR_LOG("Failed to record command buffer");
        return COMMAND_ERROR;
    }

    return SUCCESS;
}

Return_t Object::init_buffers() {
    MSG_LOG("Initalizing object buffers...");

    MSG_LOG("Init success!");
    return SUCCESS;
}

Return_t Object::init_pipeline(VkData* vk, RenderData* rd, DeletionQueue* deleteQueue) {
    std::string msg = "Initalizing material pipeline.";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    msg += '.';

    if (vk->dispTable.createPipelineLayout(&pipelineLayoutInfo, nullptr, &material.layout) != VK_SUCCESS) {
        ERR_LOG(msg +" Fail!\n\tFailed to create triangle pipeline layout");
        return PIPELINE_ERROR;
    }
    msg += '.';

    VkShaderModule vertModule, fragModule;
    if (load_shader(vk, (std::string)SHADER_DIRECTORY + "/triangle.vert.spv", &vertModule) != SUCCESS) {
        ERR_LOG(msg +" Fail!\n\tFailed to create triangle vertex shader module");
        return PIPELINE_ERROR;
    }
    msg += '.';
    if (load_shader(vk, (std::string)SHADER_DIRECTORY + "/triangle.frag.spv", &fragModule) != SUCCESS) {
        ERR_LOG(msg +" Fail!\n\tFailed to create triangle fragment shader module");
        return PIPELINE_ERROR;
    }
    msg += '.';

    VkPipelineShaderStageCreateInfo vertStageInfo = {};
    vertStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    vertStageInfo.module = vertModule;
    vertStageInfo.pName  = "main";
    msg += '.';

    VkPipelineShaderStageCreateInfo fragStageInfo = {};
    fragStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStageInfo.module = fragModule;
    fragStageInfo.pName  = "main";
    msg += '.';

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertStageInfo, fragStageInfo };
    msg += '.';

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    { // Local scope for getting vertex descriptions
        Vertex vertex;
        auto bindingDescriptions    = vertex.getBindingDescriptions();
        auto attributeDescriptions  = vertex.getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount   = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions      = bindingDescriptions.data();
        vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
    }
    msg += '.';

    // vertexInputInfo.vertexBindingDescriptionCount   = 0;
    // vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    msg += '.';

    VkViewport viewport = {};
    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = (float)vk->swapchain.extent.width;
    viewport.height     = (float)vk->swapchain.extent.height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;
    msg += '.';

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = vk->swapchain.extent;
    msg += '.';

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports    = &viewport;
    viewportState.scissorCount  = 1;
    viewportState.pScissors     = &scissor;
    msg += '.';

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType                    = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable         = VK_FALSE;
    rasterizer.rasterizerDiscardEnable  = VK_FALSE;
    rasterizer.polygonMode              = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth                = 1.0f;
    rasterizer.cullMode                 = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace                = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable          = VK_FALSE;
    msg += '.';

    // Disable all depth testing.
    VkPipelineDepthStencilStateCreateInfo depthStencil{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
    msg += '.';

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable   = VK_FALSE;
    multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    msg += '.';

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable    = VK_FALSE;
    msg += '.';

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable     = VK_FALSE;
    colorBlending.logicOp           = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount   = 1;
    colorBlending.pAttachments      = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    msg += '.';

    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    msg += '.';

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount   = static_cast<uint32_t>(dynamicStates.size());
    dynamicInfo.pDynamicStates      = dynamicStates.data();
    msg += '.';

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount             = 2;
    pipelineInfo.pStages                = shaderStages;
    pipelineInfo.pVertexInputState      = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState    = &inputAssembly;
    pipelineInfo.pViewportState         = &viewportState;
    pipelineInfo.pRasterizationState    = &rasterizer;
    pipelineInfo.pMultisampleState      = &multisampling;
    pipelineInfo.pColorBlendState       = &colorBlending;
    pipelineInfo.pDynamicState          = &dynamicInfo;
    pipelineInfo.layout                 = material.layout;
    pipelineInfo.renderPass             = rd->renderPass;
    pipelineInfo.subpass                = 0;
    pipelineInfo.basePipelineHandle     = VK_NULL_HANDLE;
    msg += '.';

    if (vk->dispTable.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &material.pipeline) != VK_SUCCESS) {
        ERR_LOG(msg +" Fail!\n\tFailed to create triangle pipeline");
        return PIPELINE_ERROR;
    }
    msg += '.';

    vk->dispTable.destroyShaderModule(fragModule, nullptr);
    vk->dispTable.destroyShaderModule(vertModule, nullptr);
    msg += '.';

    deleteQueue->add([&] {
        vk->dispTable.destroyPipeline(material.pipeline, nullptr);
        vk->dispTable.destroyPipelineLayout(material.layout, nullptr);
        // vk->dispTable.destroyRenderPass(material.renderPass, nullptr);
    });
    msg += '.';

    MSG_LOG(msg +" Success");
    return SUCCESS;
}

Return_t Object::init_default_data() {
    std::string msg = "Initalizing object default data.";

    vertices = TRIANGLE_VERTICES;
    indices  = TRIANGLE_INDICES;
    msg += '.';

    // vertexBuffer.usage = VkBufferUsageFlagBits(
    //     VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 
    //     VK_BUFFER_USAGE_TRANSFER_DST_BIT | 
    //     VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);
    // vertexBuffer.vmaUsage = VMA_MEMORY_USAGE_GPU_ONLY;
    // msg += '.';

    // indexBuffer.usage = VkBufferUsageFlagBits(
    //     VK_BUFFER_USAGE_INDEX_BUFFER_BIT | 
    //     VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    // indexBuffer.vmaUsage = VMA_MEMORY_USAGE_GPU_ONLY;
    // msg += '.';

    MSG_LOG(msg +" Success");
    return SUCCESS;
}