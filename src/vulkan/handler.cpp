#include "handler.hpp"
/**
 * @file vulkan/handler.cpp
 * @author Ray Richter
 * @brief VkHandler class definitions.
 */

/// @section Constructors

VkHandler::VkHandler(
const WindowInit &windowInit, 
const char* appName, 
const uint32_t &maxFramesInFlight, 
const std::vector<Vertex> &vertices,
const std::vector<uint32_t> &indices) : 
    // Sub class constructor calls in ascending order
    sync_           (&swapchain_, maxFramesInFlight),
    descriptors_    (&uboManager_, &descLayout_),
    uboManager_     (&device_, maxFramesInFlight),
    indexBuffer_    (&cmdPool_, indices),
    vertexBuffer_   (&cmdPool_, vertices),
    cmdPool_        (&renderPass_, maxFramesInFlight, swapchain_.getQFamIdx().graphicsFamily.value()),
    framebuffers_   (&renderPass_),
    pipeline_       (&renderPass_, &descLayout_),
    descLayout_     (&device_),
    renderPass_     (&swapchain_),
    swapchain_      (&device_),
    device_         (&instance_, &surface_),
    surface_        (&instance_, &window_, maxFramesInFlight),
    instance_       (VALIDATION_LAYERS, appName, VALIDATION_LAYERS_EN),
    window_         (windowInit)
{
    DEBUG_MSG("Called VkHandler initializer constructor");
    // VK_CHECK(init_(windowInit, appName, maxFramesInFlight));
    currentFrame_ = 0;
    maxFIF_ = maxFramesInFlight;
}

VkHandler::VkHandler() {
    DEBUG_MSG("Called VkHandler default constructor");
}

VkHandler::~VkHandler() {
    DEBUG_MSG("Called VkHandler deconstructor");

    // // Cleanup framebuffers
    // framebuffers_.~FrameBufferManager();

    // // CLeanup swapchain
    // swapchain_.~SwapchainManager();
    
    // // Cleanup every uniform buffer
    // uboManager_.~UniformBufferManager();

    // // Destroy descriptor pool
    // descriptors_.~DescriptorManager();

    // // Destroy descriptor layout
    // descLayout_.~DescriptorLayoutManager();
    
    // // Cleanup buffers
    // indexBuffer_.~BufferManager();
    // vertexBuffer_.~BufferManager();
    
    // // Destroy pipeline and pipeline layout
    // pipeline_.~PipelineManager();

    // // Destroy render pass
    // renderPass_.~RenderPassManager();

    // // Destroy the semaphores and fences when all commands have finished and no more syncronization is necessary
    // sync_.~SyncManager();

    // // Destroy the command pool
    // cmdPool_.~CommandPoolManager();
    
    // // Destroy devices
    // device_.~DeviceManager();

    // // Destroy the surface and validation layers
    // surface_.~SurfaceManager();

    // // Destroy the instance
    // instance_.~VulkanInstance();

    // // Destroy the window and terminate GLFW
    // window_.~WindowManager();
}

/// @section Public Member Functions

VkResult VkHandler::init(
const WindowInit &windowInit, 
const char* appName, 
const uint32_t &maxFramesInFlight, 
const std::vector<Vertex> &vertices,
const std::vector<uint32_t> &indices) {
    DEBUG_MSG("Initalizing VkHandler with a function...\n");
    VK_CHECK(init_(windowInit, appName, maxFramesInFlight, vertices, indices));
    return VK_SUCCESS;
}

void VkHandler::cleanup() {
    swapchain_.cleanup();

    // Cleanup every uniform buffer
    uboManager_.cleanup();

    // Descriptor sets get destroyed when the descriptor pool is destroyed
    descriptors_.cleanup();

    // Cleanup descriptor set layout
    descLayout_.cleanup();

    // Cleanup buffers
    indexBuffer_.cleanup();
    vertexBuffer_.cleanup();

    // Destroy pipeline and pipeline layout
    pipeline_.cleanup();

    // Cleanup render pass
    renderPass_.cleanup();

    // Destroy the semaphores and fences when all commands have finished and no more syncronization is necessary
    sync_.cleanup();

    // Destroy the command pool
    cmdPool_.cleanup();
    
    // Destroy the device
    device_.cleanup();

    // Destroy surface
    surface_.cleanup();

    // Destroy instance
    instance_.cleanup();

    // Destroy window and glfw
    window_.cleanup();
}

void VkHandler::mainLoop() {
    glfwPollEvents();
    drawFrame();
}

void VkHandler::endLoop() {
    vkDeviceWaitIdle(device_.getDevice());
}

void VkHandler::drawFrame() {
    VkResult result;
    
    // Wait for the previous frame then manually reset them
    // sync_.wait(1, currentFrame_, VK_TRUE, UINT64_MAX);
    vkWaitForFences(
        device_.getDevice(), 
        1, 
        sync_.getPFence(currentFrame_), 
        VK_TRUE, 
        UINT64_MAX);

    // Fix for a deadlock
    uint32_t imageIndex;
    // result = sync_.getNextImage(UINT64_MAX, currentFrame_, VK_NULL_HANDLE, &imageIndex);
    result = vkAcquireNextImageKHR(
        device_.getDevice(), 
        swapchain_.getSwapchain(), 
        UINT64_MAX, 
        sync_.getImageSemaphore(currentFrame_), 
        VK_NULL_HANDLE, 
        &imageIndex);

    // if result == VK_SUBOPTIMAL_KHR, the swapchain can still be used but the surface properties are not matched correctly
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        DEBUG_MSG("Next image out of date!");
        recreateSwapchain_();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        VK_CHECK(result);
    }

    uboManager_.update(currentFrame_, swapchain_.getScExtent());

    // Only reset the fences if we are submitting work
    sync_.resetFences(1, currentFrame_);

    // Reset then record the command buffer
    // vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    cmdPool_.resetCB(currentFrame_);
    result = recordCommandBuffer(currentFrame_, imageIndex);
    if (result != VK_SUCCESS) return;

    // Submit the command buffer
    // Queue submission and syncronization is configured in submitInfo
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Specify which semaphores to wait on before execution begins
    VkSemaphore waitSemaphores[]    = {sync_.getImageSemaphore(currentFrame_)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;

    // Specify which command buffers to submit for execution
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &cmdPool_.getCmdBuffers()[currentFrame_];

    // Specify which semaphores to signal once the command buffer(s) have finished execution
    VkSemaphore signalSemaphores[]  = {sync_.getRenderSemaphore(currentFrame_)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    // Submit the command buffer to the graphics queue
    result = vkQueueSubmit(swapchain_.getGraphicsQueue(), 1, &submitInfo, sync_.getFence(currentFrame_));
    if (result != VK_SUCCESS) return;

    // Submit the results back to the swapchain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    // Specify which semaphores to wait on
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;

    // Specify the swapchain to present images to
    VkSwapchainKHR swapchains[]     = {swapchain_.getSwapchain()};
    presentInfo.swapchainCount      = 1;
    presentInfo.pSwapchains         = swapchains;
    presentInfo.pImageIndices       = &imageIndex;
    // Can be sent an array of VK_RESULTS to check if every swapchain presentation was successful
    presentInfo.pResults = nullptr; // Optional

    // Submit the request to present an image to the swapchain
    result = vkQueuePresentKHR(swapchain_.getPresentQueue(), &presentInfo);

    // Check if the image has been resized
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.resized()) {
        DEBUG_MSG("Window resized!");
        window_.notResize();
        recreateSwapchain_();
    } else if (result != VK_SUCCESS) {
        VK_CHECK(result);
    }

    // Advance to the next frame
    currentFrame_ = (currentFrame_ + 1) % maxFIF_;
}

/// @section Private Member Functions

VkResult VkHandler::init_(
const WindowInit &windowInit, 
const char* appName, 
const uint32_t &maxFramesInFlight, 
const std::vector<Vertex> &vertices,
const std::vector<uint32_t> &indices) {
    window_         .init(windowInit);
    instance_       .init(VALIDATION_LAYERS, appName, VALIDATION_LAYERS_EN);
    surface_        .init(&instance_, &window_, maxFramesInFlight);
    device_         .init(&instance_, &surface_);
    swapchain_      .init(&device_);
    renderPass_     .init(&swapchain_);
    descLayout_     .init(&device_);
    pipeline_       .init(&renderPass_, &descLayout_);
    framebuffers_   .init(&renderPass_);
    cmdPool_        .init(&renderPass_, maxFramesInFlight, swapchain_.getQFamIdx().graphicsFamily.value());
    vertexBuffer_   .init(&cmdPool_, vertices);
    indexBuffer_    .init(&cmdPool_, indices);
    uboManager_     .init(&device_, maxFramesInFlight);
    descriptors_    .init(&uboManager_, &descLayout_);
    sync_           .init(&swapchain_, maxFramesInFlight);

    currentFrame_ = 0;
    maxFIF_ = maxFramesInFlight;
    DEBUG_MSG("\nVkHandler: Init done!");
    return VK_SUCCESS;
}

VkResult VkHandler::recordCommandBuffer(const uint32_t &currentFrame, const uint32_t &imageIndex) {
    VkResult result;
    VkCommandBuffer commandBuffer = cmdPool_.getCmdBuffer(currentFrame);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    if (result != VK_SUCCESS) return result;

    // Bind the framebuffer for the swapchain image we want to draw
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass           = renderPass_.getRenderPass();
    renderPassInfo.framebuffer          = framebuffers_.getFramebuffer(imageIndex);
    // Define the size of the render area
    renderPassInfo.renderArea.offset    = {0, 0};
    renderPassInfo.renderArea.extent    = swapchain_.getScExtent();

    // Define clear values for VK_ATTACHMENT_LOAD_OP_CLEAR
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    // Begin render pass
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Basic drawing commands:

        // Bind the graphics pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_.getPipeline());

        // Since viewport and scissor state are dynamic, set them in the command buffer before issuing a draw command
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width  = (float) swapchain_.getScExtent().width;
        viewport.height = (float) swapchain_.getScExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain_.getScExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        
        // Bind vertex buffers
        VkBuffer vertexBuffers[] = {vertexBuffer_.getBuffer()};
        VkDeviceSize   offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        // Bind index buffers
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer_.getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        // Bind the descriptor set each frame to the descriptors in the shader
        vkCmdBindDescriptorSets(commandBuffer, 
            VK_PIPELINE_BIND_POINT_GRAPHICS, 
            pipeline_.getLayout(), 
            0, 
            1, 
            descriptors_.getPSet(currentFrame), 
            0, 
            nullptr);

        // Issue a draw command using indicies
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer_.size()), 1, 0, 0, 0);

    // End the render pass
    vkCmdEndRenderPass(commandBuffer);

    // End the command
    result = vkEndCommandBuffer(commandBuffer);
    return result;
}

VkResult VkHandler::recreateSwapchain_() {
    VkResult result;
    DEBUG_MSG("Recreating swapchain...");

    // Handle minimization
    int width = 0, height = 0;
    glfwGetFramebufferSize(window_.getWindow(), &width, &height);
    // Idle while the window is minimized
    if (width == 0 || height == 0) { DEBUG_MSG("Window minimized"); }
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window_.getWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device_.getDevice());
    // while (vkDeviceWaitIdle(device_.getDevice()) != VK_SUCCESS) {}
    // result = vkDeviceWaitIdle(device_.getDevice());
    // if (result != VK_SUCCESS) { DEBUG_MSG("Recreate Swapchain ERROR: Device wait idle failed! Vulkan error: " << result); return result; }

    result = framebuffers_.cleanup();
    if (result != VK_SUCCESS) { DEBUG_MSG("Recreate Swapchain ERROR: Cleanup framebuffers failed! Vulkan error: " << result); return result; }

    result = swapchain_.cleanup();
    if (result != VK_SUCCESS) { DEBUG_MSG("Recreate Swapchain ERROR: Cleanup swapchain failed! Vulkan error: " << result); return result; }

    result = swapchain_.init(&device_);
    if (result != VK_SUCCESS) { DEBUG_MSG("Recreate Swapchain ERROR: Init swapchain failed! Vulkan error: " << result); return result; }

    result = framebuffers_.init(&renderPass_);
    if (result != VK_SUCCESS) { DEBUG_MSG("Recreate Swapchain ERROR: Init framebuffers failed! Vulkan error: " << result); return result; }

    DEBUG_MSG("Swapchain recreated!");
    return result;
}