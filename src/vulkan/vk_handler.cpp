#include "vk_handler.hpp"

VkHandler::VkHandler() {

}


VkHandler::~VkHandler() {
    swapchain.~VkSwapchainClass();

    // Cleanup every uniform buffer
    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     vkDestroyBuffer(device, uniformBuffers[i], nullptr);
    //     vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    // }

    // // Descriptor sets get destroyed when the descriptor pool is destroyed
    // vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    // vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    // vkDestroyBuffer(device, indexBuffer, nullptr);
    // vkFreeMemory(device, indexBufferMemory, nullptr);

    // vkDestroyBuffer(device, vertexBuffer, nullptr);
    // vkFreeMemory(device, vertexBufferMemory, nullptr);

    // // Destroy pipeline and pipeline layout
    // vkDestroyPipeline(device, graphicsPipeline, nullptr);
    // vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    renderPass.~VkRenderPassClass();

    // // Destroy the semaphores and fences when all commands have finished and no more syncronization is necessary
    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    //     vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    //     vkDestroyFence(device, inFlightFences[i], nullptr);
    // }

    // // Destroy the command pool
    // vkDestroyCommandPool(device, commandPool, nullptr);
    
    // Destroy devices
    devices.~VkDevicesClass();

    // Destroy the surface and validation layers
    surface.~VkSurfaceClass();
    // Destroy the instance
    instance.~VkInstanceClass();

    // Destroy the window and terminate GLFW
    window.~VkWindowClass();
}

/// @brief Initalizer for Vulkan
/// @param windowParams Inital parameters for a window
/// @return VkResult
VkResult VkHandler::init(const WindowInit *windowParams) {
    // Initalize a window
    VK_CHECK(window.init(windowParams));
    // Initalize an instance
    VK_CHECK(instance.init());
    // Initalize a surface
    VK_CHECK(surface.init(&instance, &window));
    // Initalize a physical device and create a logical device
    VK_CHECK(devices.init(&surface));
    // Initalize a swapchain
    VK_CHECK(swapchain.init(&devices));
    // Initalize the render pass
    VK_CHECK(renderPass.init(&devices, &swapchain));
    // Initalize a descriptor set
    VK_CHECK(descriptors.init(&devices));
    // Initalize the graphics pipeline
    VK_CHECK(gfxPipeline.init(&descriptors));
    
    // createFramebuffers();
    // createCommandPool();
    // createVertexBuffer();
    // createIndexBuffer();
    // createUniformBuffers();
    // createDescriptorPool();
    // createDescriptorSets();
    // createCommandBuffers();
    // createSyncObjects();

    return VK_SUCCESS;
}