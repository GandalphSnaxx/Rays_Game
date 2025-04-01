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

#ifdef _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define MSG_LOG(m) std::cout << "[MATERIAL] Message in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl
#define ERR_LOG(m) std::cout << "[MATERIAL] ERROR   in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl

std::array<VkVertexInputBindingDescription, 1> Vertex::getBindingDescription() {
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
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

Return_t Object::draw(VkCommandBuffer& cmd, const VkRenderPass& renderPass, const VkFramebuffer& fb, const VkExtent2D& extent) {
    // //begin a render pass  connected to our draw image
	// VkRenderingAttachmentInfo colorAttachment = {};
    // colorAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    // colorAttachment.pNext       = nullptr;
    // colorAttachment.imageView   = view;
    // colorAttachment.imageLayout = layout;
    // colorAttachment.loadOp      = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    // colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    // if (clear) {
    //     colorAttachment.clearValue = *clear;
    // }
    // // vkinit::attachment_info(_drawImage.imageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

	// // VkRenderingInfo renderInfo = vkinit::rendering_info(_drawExtent, &colorAttachment, nullptr);
    // VkRenderingInfo renderInfo      = {};
    // renderInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    // renderInfo.pNext                = nullptr;
    // renderInfo.renderArea           = VkRect2D { VkOffset2D { 0, 0 }, renderExtent };
    // renderInfo.layerCount           = 1;
    // renderInfo.colorAttachmentCount = 1;
    // renderInfo.pColorAttachments    = &colorAttachment;
    // renderInfo.pDepthAttachment     = &depthAttachment;
    // renderInfo.pStencilAttachment   = nullptr;

	// vkCmdBeginRendering(cmd, &renderInfo);

	// vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material.pipeline);

	// //set dynamic viewport and scissor
	// VkViewport viewport = {};
	// viewport.x = 0;
	// viewport.y = 0;
	// viewport.width = _drawExtent.width;
	// viewport.height = _drawExtent.height;
	// viewport.minDepth = 0.f;
	// viewport.maxDepth = 1.f;

	// vkCmdSetViewport(cmd, 0, 1, &viewport);

	// VkRect2D scissor = {};
	// scissor.offset.x = 0;
	// scissor.offset.y = 0;
	// scissor.extent.width = _drawExtent.width;
	// scissor.extent.height = _drawExtent.height;

	// vkCmdSetScissor(cmd, 0, 1, &scissor);

	// //launch a draw command to draw 3 vertices
	// vkCmdDraw(cmd, 3, 1, 0, 0);

	// vkCmdEndRendering(cmd);

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
    renderPassInfo.renderArea.offset    = {0, 0};
    renderPassInfo.renderArea.extent    = extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

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
        scissor.offset   = {0, 0};
        scissor.extent   = extent;
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { vertexBuffer.handle };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(cmd, indexBuffer.handle, 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(cmd, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(cmd);

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        ERR_LOG("Failed to record command buffer");
        return COMMAND_ERROR;
    }

    return SUCCESS;
}