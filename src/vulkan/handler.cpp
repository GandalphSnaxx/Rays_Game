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

    // Cleanup framebuffers
    framebuffers_.~FrameBufferManager();

    // CLeanup swapchain
    swapchain_.~SwapchainManager();
    
    // Cleanup every uniform buffer
    uboManager_.~UniformBufferManager();

    // Destroy descriptor pool
    descriptors_.~DescriptorManager();

    // Destroy descriptor layout
    descLayout_.~DescriptorLayoutManager();
    
    // Cleanup buffers
    indexBuffer_.~BufferManager();
    vertexBuffer_.~BufferManager();
    
    // Destroy pipeline and pipeline layout
    pipeline_.~PipelineManager();

    // Destroy render pass
    renderPass_.~RenderPassManager();

    // Destroy the semaphores and fences when all commands have finished and no more syncronization is necessary
    sync_.~SyncManager();

    // Destroy the command pool
    cmdPool_.~CommandPoolManager();
    
    // Destroy devices
    device_.~DeviceManager();

    // Destroy the surface and validation layers
    surface_.~SurfaceManager();

    // Destroy the instance
    instance_.~VulkanInstance();

    // Destroy the window and terminate GLFW
    window_.~WindowManager();
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
    sync_.wait(1, currentFrame_, VK_TRUE, UINT64_MAX);

    // Fixing a deadlock
    uint32_t imageIndex;
    result = sync_.getNextImage(UINT64_MAX, currentFrame_, VK_NULL_HANDLE, &imageIndex);

    // if result == VK_SUBOPTIMAL_KHR, the swapchain can still be used but the surface properties are not matched correctly
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapchain_.remake(); // recreateSwapChain();
        indexBuffer_.remake();
        framebuffers_.remake();
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
    VkSemaphore waitSemaphores[] = {sync_.getImageAvailableSemaphore(currentFrame_)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    VkCommandBuffer cmdBuffers[] = {cmdPool_.getCmdBuffer(currentFrame_)};
    // Specify which command buffers to submit for execution
    submitInfo.commandBufferCount = 1;
    // submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    submitInfo.pCommandBuffers = cmdBuffers;

    // Specify which semaphores to signal once the command buffer(s) have finished execution
    VkSemaphore signalSemaphores[] = {sync_.getRenderFinishedSemaphore(currentFrame_)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit the command buffer to the graphics queue
    result = vkQueueSubmit(swapchain_.getGraphicsQueue(), 1, &submitInfo, sync_.getInFlightFence(currentFrame_));

    // Submit the results back to the swapchain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    // Specify which semaphores to wait on
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Specify the swapchain to present images to
    VkSwapchainKHR swapChains[] = {swapchain_.getSwapchain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    // Can be sent an array of VK_RESULTS to check if every swapchain presentation was successful
    presentInfo.pResults = nullptr; // Optional

    // Submit the request to present an image to the swapchain
    result = vkQueuePresentKHR(swapchain_.getPresentQueue(), &presentInfo);

    // Check if the image has been resized
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.resized()) {
        window_.notResize();
        swapchain_.remake();
        indexBuffer_.remake();
        framebuffers_.remake();
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

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    result = vkBeginCommandBuffer(cmdPool_.getCmdBuffer(currentFrame), &beginInfo);
    if (result != VK_SUCCESS) return result;

    // Bind the framebuffer for the swapchain image we want to draw
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass           = renderPass_.getRenderPass();
    renderPassInfo.framebuffer          = framebuffers_.getFramebuffer(currentFrame);
    // Define the size of the render area
    renderPassInfo.renderArea.offset    = {0, 0};
    renderPassInfo.renderArea.extent    = renderPass_.getScExtent();

    // Define clear values for VK_ATTACHMENT_LOAD_OP_CLEAR
    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    // Begin render pass
    vkCmdBeginRenderPass(cmdPool_.getCmdBuffer(currentFrame), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Basic drawing commands:

        // Bind the graphics pipeline
        vkCmdBindPipeline(cmdPool_.getCmdBuffer(currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_.getPipeline());

        // Since viewport and scissor state are dynamic, set them in the command buffer before issuing a draw command
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width  = (float) renderPass_.getScExtent().width;
        viewport.height = (float) renderPass_.getScExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(cmdPool_.getCmdBuffer(currentFrame), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = renderPass_.getScExtent();
        vkCmdSetScissor(cmdPool_.getCmdBuffer(currentFrame), 0, 1, &scissor);
        
        // Bind vertex buffers
        VkBuffer vertexBuffers[] = {vertexBuffer_.getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(cmdPool_.getCmdBuffer(currentFrame), 0, 1, vertexBuffers, offsets);

        // Bind index buffers
        vkCmdBindIndexBuffer(cmdPool_.getCmdBuffer(currentFrame), indexBuffer_.getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        VkDescriptorSet descSet[] = {descriptors_[currentFrame]};
        // Bind the descriptor set each frame to the descriptors in the shader
        vkCmdBindDescriptorSets(cmdPool_.getCmdBuffer(currentFrame), 
            VK_PIPELINE_BIND_POINT_GRAPHICS, 
            pipeline_.getLayout(), 
            0, 
            1, 
            descSet, 
            0, 
            nullptr);

        // Issue a draw command using indicies
        vkCmdDrawIndexed(cmdPool_.getCmdBuffer(currentFrame), static_cast<uint32_t>(indexBuffer_.size()), 1, 0, 0, 0);

    // End the render pass
    vkCmdEndRenderPass(cmdPool_.getCmdBuffer(currentFrame));

    // End the command
    result = vkEndCommandBuffer(cmdPool_.getCmdBuffer(currentFrame));
    return result;
}