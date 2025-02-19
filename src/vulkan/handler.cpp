#include "handler.hpp"

/// @brief Constructor for the Vulkan Handler. This will call constructors for each parent class
/// @param window_init Initial window parameters (ie. width, height, title)
/// @param appName Name of the application
VkHandlerClass::VkHandlerClass(const WindowInit *window_init, const char *appName, uint32_t max_frames_in_flight) : 
    // Call constructors in ascending order
    VkPipelineClass(SHADER_FILES),  // Create graphics pipeline
    VkPipelineOverheadClass(SHADER_FILES),
    VkUBOOverheadClass(),           // Create descriptor set layout
    VkSwapchainClass(),             // Create swapchain, create image views, create render pass
    VkDeviceClass(),                // Pick physical device, create logistical device
    VkSurfaceClass(),               // Create surface
    VkInstanceClass(appName),       // Create instance, setup debug messenger
    VkWindowClass(window_init)      // Init window
{
    DEBUG_MSG("Called VkHandlerClass constructor");
    
    // createWindow();
    // createInstance();
    // setupDebugMessenger();
    // createSurface();
    // pickPhysicalDevice();
    // createLogicalDevice();
    // createSwapChain();
    // createImageViews();
    // createRenderPass();
    // createDescriptorSetLayout();
    // createGraphicsPipeline();
    
    // createFramebuffers();

    // createCommandPool();
    // createVertexBuffer();
    // createIndexBuffer();
    // createUniformBuffers();
    // createDescriptorPool();
    // createDescriptorSets();
    // createCommandBuffers();
    // createSyncObjects();
}

VkHandlerClass::~VkHandlerClass() {
    DEBUG_MSG("Called VkHandlerClass deconstructor");

    // swapchain.~VkSwapchainClass();

    // // Cleanup every uniform buffer
    // // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    // //     vkDestroyBuffer(device, uniformBuffers[i], nullptr);
    // //     vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    // // }

    // // // Descriptor sets get destroyed when the descriptor pool is destroyed
    // // vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    // // vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    // // vkDestroyBuffer(device, indexBuffer, nullptr);
    // // vkFreeMemory(device, indexBufferMemory, nullptr);

    // // vkDestroyBuffer(device, vertexBuffer, nullptr);
    // // vkFreeMemory(device, vertexBufferMemory, nullptr);

    // // // Destroy pipeline and pipeline layout
    // // vkDestroyPipeline(device, graphicsPipeline, nullptr);
    // // vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    // renderPass.~VkRenderPassClass();

    // // // Destroy the semaphores and fences when all commands have finished and no more syncronization is necessary
    // // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    // //     vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    // //     vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    // //     vkDestroyFence(device, inFlightFences[i], nullptr);
    // // }

    // // // Destroy the command pool
    // // vkDestroyCommandPool(device, commandPool, nullptr);
    
    // // Destroy devices
    // devices.~VkDevicesClass();

    // // Destroy the surface and validation layers
    // surface.~VkSurfaceClass();
    // // Destroy the instance
    // instance.~VkInstanceClass();

    // // Destroy the window and terminate GLFW
    // window.~VkWindowClass();
}

