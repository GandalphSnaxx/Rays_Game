/**
 * @file vk_engine.cpp
 * @author Ray Richter
 * @brief Vulkan engine definitions
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "vk_engine.hpp"
#include <fstream>
#include <iostream>
// #include "constants.hpp"
#include "../utils/utils.hpp"

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#ifdef _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

// #define MSG_LOG(m) message_log_(m, __FILENAME__, __LINE__)
// #define ERR_LOG(m) error_log_(m, __FILENAME__, __LINE__)

#define MSG_LOG(m) std::cout << "[ENGINE] Message in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl
#define ERR_LOG(m) std::cout << "[ENGINE] ERROR   in " << __FILENAME__ << " at line " << __LINE__ << "\t - \"" << m << "\"" << std::endl

#define ERR_CHECK(f, e)         \
if(f < 0) {                     \
    ERR_LOG(#f << ": Fail");    \
    return e;                   \
} else {                        \
    MSG_LOG(#f << ": Success"); \
}

/// @brief Returns `true` if Return_t is NOT an error
bool operator!(const Return_t& ret) {
    // std::cout << ret << " >= SUCCESS: " << (ret >= SUCCESS ? "TRUE" : "FALSE") << std::endl;
    return ret >= SUCCESS;
}

using namespace vk;

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Public Member Functions
//---------------------------------------------------------------------------------------------------------------------------//

Return_t Engine::init(const VkInit& init /* = {} */) {
    MSG_LOG("Initalizing Vulkan engine...");

    init_ = init;
    if(init_.validationLayersEnable) {
        MSG_LOG("Vulkan validation layers enabled");
        flags_ += VALIDATION_LAYERS; // Add Vulkan validation layers
    }

    // Initalize each part
    // if (!init_sdl_()) {
    //     ERR_LOG("Failed to initalize SDL");
    //     return SDL_ERROR;
    // } else {
    //     MSG_LOG("SDL initalized");
    // }
    ERR_CHECK(init_sdl_(),              SDL_ERROR           );
    ERR_CHECK(init_vulkan_(),           VULKAN_ERROR        );
    ERR_CHECK(init_swapchain_(),        SWAPCHAIN_ERROR     );
    ERR_CHECK(init_queues_(),           SYNC_ERROR          );
    ERR_CHECK(init_render_pass_(),      RENDER_PASS_ERROR   );
    ERR_CHECK(init_pipelines_(),        PIPELINE_ERROR      );
    ERR_CHECK(init_framebuffers_(),     FRAME_ERROR         );
    ERR_CHECK(init_command_pools_(),    COMMAND_ERROR       );
    ERR_CHECK(init_command_buffers_(),  COMMAND_ERROR       );
    ERR_CHECK(init_sync_(),             SYNC_ERROR          );

    vk_.dispTable.deviceWaitIdle();

    flags_ += INITALIZED; // Everything went correctly
    MSG_LOG("Init success");
    return SUCCESS;
}

Return_t Engine::draw(const SDL_Event& event) {
    // Handle SDL events
    switch (event.type) {
        case SDL_EVENT_QUIT:
            flags_ += SHUTDOWN_REQUESTED;
            break;
    }

    // Begin the current frames command buffer
    // call draw background
    // call draw triangle
    // VkCommandBuffer cmd = get_active_command_buffer_();
    vk_.dispTable.waitForFences(1, &renderData_.inFlightFences[renderData_.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex = 0;
    VkResult result = vk_.dispTable.acquireNextImageKHR(
        vk_.swapchain, 
        UINT64_MAX, 
        renderData_.availableSemaphores[renderData_.currentFrame], 
        VK_NULL_HANDLE, 
        &imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        return recreate_swapchain_();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        ERR_LOG("Failed to acquire swapchain image");
        return ERROR;
    }

    if (renderData_.imageInFlight[imageIndex] != VK_NULL_HANDLE) {
        vk_.dispTable.waitForFences(1, &renderData_.imageInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    renderData_.imageInFlight[imageIndex] = renderData_.imageInFlight[renderData_.currentFrame];

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = { renderData_.availableSemaphores[renderData_.currentFrame] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = wait_semaphores;
    submitInfo.pWaitDstStageMask    = wait_stages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &renderData_.immCmdBuffers[imageIndex];

    VkSemaphore signal_semaphores[] = { renderData_.finishedSemaphore[renderData_.currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signal_semaphores;

    vk_.dispTable.resetFences(1, &renderData_.inFlightFences[renderData_.currentFrame]);

    if (vk_.dispTable.queueSubmit(renderData_.graphicsQueue, 1, &submitInfo, renderData_.inFlightFences[renderData_.currentFrame]) != VK_SUCCESS) {
        ERR_LOG("Failed to submit draw command buffer");
        return COMMAND_ERROR; //"failed to submit draw command buffer
    }

    VkPresentInfoKHR present_info = {};
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores    = signal_semaphores;

    VkSwapchainKHR swapchains[] = { vk_.swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains    = swapchains;
    present_info.pImageIndices  = &imageIndex;

    result = vk_.dispTable.queuePresentKHR(renderData_.presentQueue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        return recreate_swapchain_();
    } else if (result != VK_SUCCESS) {
        ERR_LOG("Failed to present swapchain image");
        return SWAPCHAIN_ERROR;
    }

    renderData_.currentFrame = (renderData_.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    return SUCCESS;
}

Return_t Engine::clean() {
    MSG_LOG("Cleaning Vulkan engine");

    if (flags_(CLEANED)) {
        MSG_LOG("Tried to clean a cleaned engine");
        return DONE_ALREADY; 
    }
    if (!flags_(INITALIZED)) {
        MSG_LOG("Tried to clean an uninitalized engine");
        return UNNECESSARY;
    }

    deleteQueue_.flush(); // Call everything in the deletion queue

    flags_ += CLEANED; // Everything went correctly
    return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Private Initalizer Functions
//---------------------------------------------------------------------------------------------------------------------------//

Return_t Engine::init_sdl_() {
    MSG_LOG("Initlalizing SDL...");

    // We initialize SDL and create a window with it.
    if(!SDL_Init(SDL_INIT_VIDEO)) {
		ERR_LOG("Failed to initialize SDL: " << SDL_GetError());
		return SDL_ERROR;
	}
    if (!SDL_Vulkan_LoadLibrary(nullptr)) {
        ERR_LOG("Failed to load Vulkan library for SDL: " << SDL_GetError());
        return SDL_ERROR;
    }

    // Use SDL to create a window
    vk_.window = SDL_CreateWindow(
        init_.appName,
        init_.defaultWindowSize.width,
        init_.defaultWindowSize.height,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    // Check for errors
    if (vk_.window == nullptr) {
        ERR_LOG("Failed to create a window: " << SDL_GetError());
        return SDL_ERROR;
    }

    // Add window destruction to the deletion queue
    deleteQueue_.add([&]() {
        SDL_DestroyWindow(vk_.window);
        SDL_Quit();
    });

    return SUCCESS;
}

Return_t Engine::init_vulkan_() {
    MSG_LOG("Initalizing Vulkan...");

    // Use VkBootstrap to initalize Vulkan
    { // Create a Vulkan instance. Get the required SDL extensions
        uint32_t extensionCount = 0;
        const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

        vkb::InstanceBuilder builder;
        auto inst_ret = builder
            .set_app_name(init_.appName)
            .request_validation_layers(flags_(VALIDATION_LAYERS))
            .enable_extensions(extensionCount, extensions)
            .use_default_debug_messenger()
            .build();

        if (!inst_ret) {
            ERR_LOG("Failed to create a Vulkan instance. Error message: " << inst_ret.error().message());
            return VK_BOOTSTRAP_ERROR;
        }
        
        // Set instance and instance dispatch table
        vk_.instance = inst_ret.value();
        vk_.instDispTable = vk_.instance.make_table();
    }
    
    // Create a surface for Vulkan
    if (!create_surface_sdl_()) {
        ERR_LOG("Failed to create a surface");
        return VK_BOOTSTRAP_ERROR;
    }

    { // Select devices
        vkb::PhysicalDeviceSelector selector(vk_.instance);
        auto phys_ret = selector
            .set_surface(vk_.surface)
            // .set_minimum_version(1, 4)
            .require_dedicated_transfer_queue()
            .select();
        if (!phys_ret) {
            ERR_LOG(("Failed to select a physical device. Error message: " + phys_ret.error().message()).c_str());
            return VK_BOOTSTRAP_ERROR;
        }

        vkb::DeviceBuilder device_builder(phys_ret.value());
        auto dev_ret = device_builder.build();
        if (!dev_ret) {
            ERR_LOG(("Failed to find a suitable GPU. Error message: " + dev_ret.error().message()).c_str());
            return VK_BOOTSTRAP_ERROR;
        }
        vk_.device = dev_ret.value();
    }

    // { // Get a graphics queue
    //     auto graphics_queue_ret = vk_.device.get_queue(vkb::QueueType::graphics);
    //     if (!graphics_queue_ret)  {
    //         ERR_LOG("Failed to get a graphics queue");
    //         return VK_BOOTSTRAP_ERROR;
    //     }
    //     renderData_.graphicsQueue = graphics_queue_ret.value();
    // }

    // Save the dispatch table
    vk_.dispTable = vk_.device.make_table();

    // Initalize the memory allocator
    VmaAllocatorCreateInfo allocInfo = ALLOC_INFO;
    allocInfo.physicalDevice    = vk_.device.physical_device;
    allocInfo.device            = vk_.device;
    allocInfo.instance          = vk_.instance;
    
    vmaCreateAllocator(&allocInfo, &allocator_);

    deleteQueue_.add([&] {
        vmaDestroyAllocator(allocator_);
        vkb::destroy_device(vk_.device);
        vkb::destroy_surface(vk_.instance, vk_.surface);
        vkb::destroy_instance(vk_.instance);
    });

    return SUCCESS;
}

Return_t Engine::init_swapchain_() {
    MSG_LOG("Initalizing swapchain...");

    vkb::SwapchainBuilder swapchain_builder{ vk_.device };
    auto swap_ret = swapchain_builder.set_old_swapchain(vk_.swapchain).build();
    if (!swap_ret) {
        ERR_LOG("Failed to build swapchain");
        return SWAPCHAIN_ERROR;
    }
    vkb::destroy_swapchain(vk_.swapchain);
    vk_.swapchain = swap_ret.value();

    deleteQueue_.add([&] {
        vkb::destroy_swapchain(vk_.swapchain);
    });

    return SUCCESS;
}

Return_t Engine::init_queues_() {
    MSG_LOG("Initalizing queues...");

    auto gfxq = vk_.device.get_queue(vkb::QueueType::graphics);
    if (!gfxq.has_value()) {
        ERR_LOG(("Failed to get graphics queue: " + gfxq.error().message()).c_str());
        return VK_BOOTSTRAP_ERROR;
    }
    renderData_.graphicsQueue = gfxq.value();

    auto pstq = vk_.device.get_queue(vkb::QueueType::present);
    if (!pstq.has_value()) {
        ERR_LOG(("Failed to get present queue: " + pstq.error().message()).c_str());
        return VK_BOOTSTRAP_ERROR;
    }
    renderData_.presentQueue = pstq.value();

    return SUCCESS;
}

Return_t Engine::init_render_pass_() {
    MSG_LOG("Initalizing render pass...");

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format          = vk_.swapchain.image_format;
    colorAttachment.samples         = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment   = 0;
    colorAttachmentRef.layout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass       = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass       = 0;
    dependency.srcStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask    = 0;
    dependency.dstStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount  = 1;
    renderPassInfo.pAttachments     = &colorAttachment;
    renderPassInfo.subpassCount     = 1;
    renderPassInfo.pSubpasses       = &subpass;
    renderPassInfo.dependencyCount  = 1;
    renderPassInfo.pDependencies    = &dependency;

    if (vk_.dispTable.createRenderPass(&renderPassInfo, nullptr, &renderData_.renderPass) != VK_SUCCESS) {
        ERR_LOG("Failed to create render pass");
        return RENDER_PASS_ERROR; // failed to create render pass!
    }

    return SUCCESS;
}

Return_t Engine::init_pipelines_() {
    MSG_LOG("Creating pipelines...");

    // Initalize the pipelines
    // ERR_CHECK(init_background_pipeline_(), PIPELINE_ERROR);
    ERR_CHECK(init_triangle_pipeline_(), PIPELINE_ERROR);

    return SUCCESS;
}

Return_t Engine::init_framebuffers_() {
    MSG_LOG("Initalizing framebuffers...");

    renderData_.swapchainImages     = vk_.swapchain.get_images().value();
    renderData_.swapchainImageViews = vk_.swapchain.get_image_views().value();

    renderData_.framebuffers.resize(renderData_.swapchainImageViews.size());

    for (size_t i = 0; i < renderData_.swapchainImageViews.size(); i++) {
        VkImageView attachments[] = { renderData_.swapchainImageViews[i] };

        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass         = renderData_.renderPass;
        framebuffer_info.attachmentCount    = 1;
        framebuffer_info.pAttachments       = attachments;
        framebuffer_info.width              = vk_.swapchain.extent.width;
        framebuffer_info.height             = vk_.swapchain.extent.height;
        framebuffer_info.layers             = 1;

        if (vk_.dispTable.createFramebuffer(&framebuffer_info, nullptr, &renderData_.framebuffers[i]) != VK_SUCCESS) {
            ERR_LOG("Failed to create a framebuffer");
            return FRAME_ERROR;
        }
    }

    deleteQueue_.add([&] {
        for (auto framebuffer : renderData_.framebuffers) {
            vk_.dispTable.destroyFramebuffer(framebuffer, nullptr);
        }
    });

    return SUCCESS;
}

Return_t Engine::init_command_pools_() {
    MSG_LOG("Initalizing commands...");

    // Create a command pool for commands submitted to the graphics queue for each frame and one for immediate submission.
    // Allow the pool to reset for individual commands
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = vk_.device.get_queue_index(vkb::QueueType::graphics).value();

    // Create the immediate command pool
    if (vk_.dispTable.createCommandPool(&poolInfo, nullptr, &renderData_.immCmdPool) != VK_SUCCESS) {
        ERR_LOG("Failed to create the immediate command pool");
        return VULKAN_ERROR;
    }

    deleteQueue_.add([&] {
        vk_.dispTable.destroyCommandPool(renderData_.immCmdPool, nullptr);
    });

    return SUCCESS;
}

Return_t Engine::init_command_buffers_() {
    MSG_LOG("Initalizing command buffers...");

    renderData_.immCmdBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool           = renderData_.immCmdPool;
    allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount    = (uint32_t)renderData_.immCmdBuffers.size();

    if (vk_.dispTable.allocateCommandBuffers(&allocInfo, renderData_.immCmdBuffers.data()) != VK_SUCCESS) {
        ERR_LOG("Failed to allocate command buffers");
        return COMMAND_ERROR;
    }

    for (int i = 0; i < renderData_.immCmdBuffers.size(); i++) {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vk_.dispTable.beginCommandBuffer(renderData_.immCmdBuffers[i], &begin_info) != VK_SUCCESS) {
            ERR_LOG("Failed to begin recording command buffer");
            return BUFFER_ERROR;
        }

        VkRenderPassBeginInfo render_pass_info = {};
        render_pass_info.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass         = renderData_.renderPass;
        render_pass_info.framebuffer        = renderData_.framebuffers[i];
        render_pass_info.renderArea.offset  = { 0, 0 };
        render_pass_info.renderArea.extent  = vk_.swapchain.extent;
        VkClearValue clearColor{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clearColor;

        VkViewport viewport = {};
        viewport.x          = 0.0f;
        viewport.y          = 0.0f;
        viewport.width      = (float)vk_.swapchain.extent.width;
        viewport.height     = (float)vk_.swapchain.extent.height;
        viewport.minDepth   = 0.0f;
        viewport.maxDepth   = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = vk_.swapchain.extent;

        vk_.dispTable.cmdSetViewport(renderData_.immCmdBuffers[i], 0, 1, &viewport);
        vk_.dispTable.cmdSetScissor(renderData_.immCmdBuffers[i], 0, 1, &scissor);

        vk_.dispTable.cmdBeginRenderPass(renderData_.immCmdBuffers[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        vk_.dispTable.cmdBindPipeline(renderData_.immCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, triangle_.material.pipeline);

        vk_.dispTable.cmdDraw(renderData_.immCmdBuffers[i], 3, 1, 0, 0);

        vk_.dispTable.cmdEndRenderPass(renderData_.immCmdBuffers[i]);

        if (vk_.dispTable.endCommandBuffer(renderData_.immCmdBuffers[i]) != VK_SUCCESS) {
            MSG_LOG("Failed to record a command buffer");
            return BUFFER_ERROR;
        }
    }

    return SUCCESS;
}

Return_t Engine::init_sync_() {
    MSG_LOG("Initalizing sync structures...");

    // Create syncronization structures
    // One fence to control when the gpu has finished rendering the frame, 
    //  and 2 semaphores to syncronize rendering with swapchain
    // Fence should start signaled so the first frame can be waited on
    renderData_.availableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderData_.finishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    renderData_.inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    renderData_.imageInFlight.resize(vk_.swapchain.image_count, VK_NULL_HANDLE);

    // VkSemaphoreCreateInfo semaphoreInfo = {};
    // semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    // VkFenceCreateInfo fenceInfo = {};
    // fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vk_.dispTable.createFence(&FENCE_INFO, nullptr, &renderData_.immFence) != VK_SUCCESS) {
        ERR_LOG("Failed to create immediate submit fence");
        return SYNC_ERROR;
    }

    deleteQueue_.add([&] {
        vk_.dispTable.destroyFence(renderData_.immFence, nullptr);
    });

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vk_.dispTable.createSemaphore(&SEMAPHORE_INFO, nullptr, &renderData_.availableSemaphores[i]) != VK_SUCCESS ||
            vk_.dispTable.createSemaphore(&SEMAPHORE_INFO, nullptr, &renderData_.finishedSemaphore[i])   != VK_SUCCESS ||
            vk_.dispTable.createFence    (&FENCE_INFO,     nullptr, &renderData_.inFlightFences[i])      != VK_SUCCESS) {
            ERR_LOG("Failed to create sync objects");
            return SYNC_ERROR;
        }

        deleteQueue_.add([&] {
            vk_.dispTable.destroySemaphore(renderData_.finishedSemaphore[i],   nullptr);
            vk_.dispTable.destroySemaphore(renderData_.availableSemaphores[i], nullptr);
            vk_.dispTable.destroyFence    (renderData_.inFlightFences[i],      nullptr);
        });
    }

    return SUCCESS;
}

// Return_t Engine::init_descriptors_() {
//     MSG_LOG("Initalizing descriptors...");

//     VkDescriptorPoolSize poolSize = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2 };
//     // std::vector<VkDescriptorPoolSize> poolSizes = {
//     //     { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          3 },
//     //     { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         3 },
//     //     { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3 },
//     // };

//     VkDescriptorPoolCreateInfo poolInfo = {};
//     poolInfo.sType          = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//     poolInfo.flags          = 0;
//     poolInfo.maxSets        = 1;
//     poolInfo.poolSizeCount  = 1;
//     poolInfo.pPoolSizes     = &poolSize;
//     vk_.dispTable.createDescriptorPool(&poolInfo, nullptr, &background_.descriptorPool);

//     VkDescriptorSetLayoutBinding binding = { 
//         .binding            = 0, 
//         .descriptorType     = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 
//         .descriptorCount    = 2, 
//         .stageFlags         = VK_SHADER_STAGE_ALL, 
//         .pImmutableSamplers = nullptr,
//     };
//     VkDescriptorSetLayoutCreateInfo dslInfo = {};
//     dslInfo.sType           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//     dslInfo.flags           = 0;
//     dslInfo.bindingCount    = 1;
//     dslInfo.pBindings       = &binding;
//     vk_.dispTable.createDescriptorSetLayout(&dslInfo, nullptr, &background_.descriptorSetLayout);

//     VkDescriptorSetAllocateInfo dsAllocateInfo = {};
//     dsAllocateInfo.sType                = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//     dsAllocateInfo.descriptorPool       = background_.descriptorPool;
//     dsAllocateInfo.descriptorSetCount   = 1;
//     dsAllocateInfo.pSetLayouts          = &background_.descriptorSetLayout;
//     vk_.dispTable.allocateDescriptorSets(&dsAllocateInfo, &background_.descriptorSet);

//     return SUCCESS;
// }

// Return_t Engine::init_default_data_() {
//     MSG_LOG("Initalizing default data...");

//     if (!init_frames_()) {
//         ERR_LOG("Failed to initalize frame data");
//         return ERROR;
//     }
//     // Create triangle buffers
//     if (!init_triangle_data_()) {
//         ERR_LOG("Failed to initalize triangle data");
//         return ERROR;
//     }

//     return SUCCESS;
// }

// Return_t Engine::init_renderables_() {
//     MSG_LOG("Initalizing renderables...");

//     return SUCCESS;
// }

// Return_t Engine::init_imgui_() {
//     MSG_LOG("Initalizing IMGUI...");

//     return SUCCESS;
// }

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Private Helper Functions
//---------------------------------------------------------------------------------------------------------------------------//

Return_t Engine::create_surface_sdl_() {
    MSG_LOG("Creating a surface with SDL...");

    // VkSurfaceKHR surface = VK_NULL_HANDLE;
    // auto err = SDL_Vulkan_CreateSurface(vk_.window, vk_.instance, nullptr, &surface);
    if (!SDL_Vulkan_CreateSurface(vk_.window, vk_.instance, nullptr, &vk_.surface)) { 
        ERR_LOG("Failed to create a surface: " << SDL_GetError());
        return SDL_ERROR; 
    }

    // vk_.surface = surface;

    return SUCCESS;
}

Return_t Engine::create_swapchain_() {
    MSG_LOG("Creating swapchain...");

    vkb::SwapchainBuilder builder(vk_.device);
    auto swap_ret = builder
        .set_old_swapchain(vk_.swapchain)
        .build();
    if (!swap_ret) {
        ERR_LOG(swap_ret.error().message().c_str());
        return VK_BOOTSTRAP_ERROR;
    }

    // Remove any old swapchain and save the new one
    vkb::destroy_swapchain(vk_.swapchain);
    vk_.swapchain = swap_ret.value();

    deleteQueue_.add([&] {
        vkb::destroy_swapchain(vk_.swapchain);
    });

    return SUCCESS;
}

// Return_t Engine::create_cmd_pool_() {
//     MSG_LOG("Creating command pools...");

//     // Create a command pool for commands submitted to the graphics queue for each frame and one for immediate submission.
//     // Allow the pool to reset for individual commands
//     VkCommandPoolCreateInfo poolInfo = {
//         .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
//         .pNext = nullptr,
//         .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
//         .queueFamilyIndex = vk_.device.get_queue_index(vkb::QueueType::graphics).value(),
//     };

//     for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//         if (vk_.dispTable.createCommandPool(&poolInfo, nullptr, &frames_[i].cmdPool) != VK_SUCCESS) {
//             ERR_LOG("Failed to create a frame based command pool");
//             return VULKAN_ERROR; // Failed to create command pool
//         }

//         // Allocate the default command buffer that is used for rendering

//         // Add command pools to their frame's deletion pool
//         frames_[i].deleteQueue.add([&]{
//             vk_.dispTable.destroyCommandPool(frames_[i].cmdPool, nullptr);
//         });
//     }

//     // Create the immediate command pool
//     if (vk_.dispTable.createCommandPool(&poolInfo, nullptr, &renderData_.immCmdPool) != VK_SUCCESS) {
//         ERR_LOG("Failed to create the immediate command pool");
//         return VULKAN_ERROR;
//     }

//     // Add frame deletion queue to the main deletion queue
//     deleteQueue_.add([&]{
//         for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//             frames_[i].deleteQueue.flush();
//         }
//         vk_.dispTable.destroyCommandPool(renderData_.immCmdPool, nullptr);
//     });
    
//     return SUCCESS;
// }

VkShaderModule createShaderModule(vkb::DispatchTable& disp, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (disp.createShaderModule(&createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

Return_t Engine::init_background_pipeline_() {
    MSG_LOG("Initalizing background pipeline...");

    auto compCode = readFile(std::string(SHADER_DIRECTORY) + "/background.comp.spv");

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = compCode.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(compCode.data());

    VkShaderModule compModule;
    vk_.dispTable.createShaderModule(&createInfo, nullptr, &compModule);

    if (compModule == VK_NULL_HANDLE) {
        ERR_LOG("Failed to create background shader module");
        return PIPELINE_ERROR;
    }

    VkPipelineShaderStageCreateInfo shaderStageInfo = {};
    shaderStageInfo.sType   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage   = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStageInfo.module  = compModule;
    shaderStageInfo.pName   = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount           = 1;
    pipelineLayoutInfo.pSetLayouts              = &background_.descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount   = 0;
    if (vk_.dispTable.createPipelineLayout(&pipelineLayoutInfo, nullptr, &background_.layout) != VK_SUCCESS) {
        ERR_LOG("Failed to create background pipeline layout");
        return PIPELINE_ERROR;
    }

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage              = shaderStageInfo;
    pipelineInfo.layout             = background_.layout;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    if (vk_.dispTable.createComputePipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &background_.pipeline) != VK_SUCCESS) {
        ERR_LOG("Failed to create background pipeline");
        return PIPELINE_ERROR;
    }

    vk_.dispTable.destroyShaderModule(compModule, nullptr);

    deleteQueue_.add([&] {
        vk_.dispTable.destroyPipeline(background_.pipeline, nullptr);
        vk_.dispTable.destroyPipelineLayout(background_.layout, nullptr);
        // vk_.dispTable.destroyRenderPass(background_.renderPass, nullptr);
    });

    return SUCCESS;
}

Return_t Engine::init_triangle_pipeline_() {
    MSG_LOG("Initalizing triangle pipeline...");

    // std::array<VkPushConstantRange, 1> pushConstantRanges;
    // pushConstantRanges[0].offset = 0;
    // pushConstantRanges[0].size = sizeof(glm::mat4) * 2;
    // pushConstantRanges[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // pipelineLayoutInfo.pPushConstantRanges      = pushConstantRanges.data();
    // pipelineLayoutInfo.pushConstantRangeCount   = (uint32_t)pushConstantRanges.size();
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    if (vk_.dispTable.createPipelineLayout(&pipelineLayoutInfo, nullptr, &triangle_.material.layout) != VK_SUCCESS) {
        ERR_LOG("Failed to create triangle pipeline layout");
        return PIPELINE_ERROR;
    }

    // auto vertCode = readFile(std::string(SHADER_DIRECTORY) + "/triangle.vert.spv");
    // auto fragCode = readFile(std::string(SHADER_DIRECTORY) + "/triangle.frag.spv");

    // VkShaderModule vertModule = createShaderModule(vk_.dispTable, vertCode);
    // VkShaderModule fragModule = createShaderModule(vk_.dispTable, fragCode);

    VkShaderModule vertModule, fragModule;
    if (!load_shader_((std::string)SHADER_DIRECTORY + "/triangle.vert.spv", &vertModule)) {
        ERR_LOG("Failed to create triangle vertex shader module");
        return PIPELINE_ERROR;
    }
    if (!load_shader_((std::string)SHADER_DIRECTORY + "/triangle.frag.spv", &fragModule)) {
        ERR_LOG("Failed to create triangle fragment shader module");
        return PIPELINE_ERROR;
    }

    // if (vertModule == VK_NULL_HANDLE || fragModule == VK_NULL_HANDLE) {
    //     ERR_LOG("Failed to create triangle shader modules");
    //     return PIPELINE_ERROR;
    // }

    VkPipelineShaderStageCreateInfo vertStageInfo = {};
    vertStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    vertStageInfo.module = vertModule;
    vertStageInfo.pName  = "main";

    VkPipelineShaderStageCreateInfo fragStageInfo = {};
    fragStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStageInfo.module = fragModule;
    fragStageInfo.pName  = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertStageInfo, fragStageInfo };

    std::array<VkVertexInputBindingDescription, 1> bindingDesc;
    bindingDesc[0].binding      = 0;
    bindingDesc[0].stride       = sizeof(glm::vec3);
    bindingDesc[0].inputRate    = VK_VERTEX_INPUT_RATE_VERTEX;

    std::array<VkVertexInputAttributeDescription, 1> attributeDesc;
    attributeDesc[0].location   = 0;
    attributeDesc[0].binding    = bindingDesc[0].binding;
    attributeDesc[0].format     = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDesc[0].offset     = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount   = (uint32_t)bindingDesc.size();
    vertexInputInfo.pVertexBindingDescriptions      = bindingDesc.data();
    vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDesc.size();
    vertexInputInfo.pVertexAttributeDescriptions    = attributeDesc.data();

    // VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    // vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // vertexInputInfo.vertexBindingDescriptionCount   = 0;
    // vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = (float)vk_.swapchain.extent.width;
    viewport.height     = (float)vk_.swapchain.extent.height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = vk_.swapchain.extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports    = &viewport;
    viewportState.scissorCount  = 1;
    viewportState.pScissors     = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType                    = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable         = VK_FALSE;
    rasterizer.rasterizerDiscardEnable  = VK_FALSE;
    rasterizer.polygonMode              = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth                = 1.0f;
    rasterizer.cullMode                 = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace                = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable          = VK_FALSE;

    // Disable all depth testing.
    VkPipelineDepthStencilStateCreateInfo depthStencil{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable   = VK_FALSE;
    multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable    = VK_FALSE;

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

    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount   = static_cast<uint32_t>(dynamicStates.size());
    dynamicInfo.pDynamicStates      = dynamicStates.data();

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
    pipelineInfo.layout                 = triangle_.material.layout;
    pipelineInfo.renderPass             = renderData_.renderPass;
    pipelineInfo.subpass                = 0;
    pipelineInfo.basePipelineHandle     = VK_NULL_HANDLE;

    if (vk_.dispTable.createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &triangle_.material.pipeline) != VK_SUCCESS) {
        ERR_LOG("Failed to create triangle pipeline");
        return PIPELINE_ERROR;
    }

    vk_.dispTable.destroyShaderModule(fragModule, nullptr);
    vk_.dispTable.destroyShaderModule(vertModule, nullptr);

    deleteQueue_.add([&] {
        vk_.dispTable.destroyPipeline(triangle_.material.pipeline, nullptr);
        vk_.dispTable.destroyPipelineLayout(triangle_.material.layout, nullptr);
        // vk_.dispTable.destroyRenderPass(triangle_.material.renderPass, nullptr);
    });

    return SUCCESS;
}

Return_t Engine::init_frames_() {
    MSG_LOG("Initalizing frame data...");

    renderData_.swapchainImages = vk_.swapchain.get_images().value();
    renderData_.swapchainImageViews = vk_.swapchain.get_image_views().value();

    renderData_.framebuffers.resize(renderData_.swapchainImageViews.size());

    for (size_t i = 0; i < renderData_.swapchainImageViews.size(); i++) {
        VkImageView attachments[] = { renderData_.swapchainImageViews[i] };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass      = renderData_.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments    = attachments;
        framebufferInfo.width           = vk_.swapchain.extent.width;
        framebufferInfo.height          = vk_.swapchain.extent.height;
        framebufferInfo.layers          = 1;

        if (vk_.dispTable.createFramebuffer(&framebufferInfo, nullptr, &renderData_.framebuffers[i]) != VK_SUCCESS) {
            ERR_LOG("Failed to create framebuffer");
            return FRAME_ERROR; // failed to create framebuffer
        }
    }

    return SUCCESS;
}

Return_t Engine::init_triangle_data_() {
    MSG_LOG("Creating triangle buffers...");

    triangle_.vertices.resize(3);
    triangle_.indices.resize(3);

    triangle_.vertices = {
        {-0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
    };

    triangle_.indices = { 0, 1, 2 };

    triangle_.vertexBuffer.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    triangle_.indexBuffer.usage  = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    create_or_resize_buffer_(triangle_.vertexBuffer, triangle_.vertices.size() * sizeof(triangle_.vertices[0]));
    create_or_resize_buffer_(triangle_.indexBuffer,  triangle_.indices.size()  * sizeof(triangle_.indices [0]));

    return SUCCESS;
}

Return_t Engine::create_cmd_buffers_() {
    MSG_LOG("Creating command buffers...");

    renderData_.immCmdBuffers.resize(renderData_.framebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = renderData_.immCmdPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)renderData_.immCmdBuffers.size();

    if (vk_.dispTable.allocateCommandBuffers(&allocInfo, renderData_.immCmdBuffers.data()) != VK_SUCCESS) {
        ERR_LOG("Failed to allocate command buffers");
        return VULKAN_ERROR;
    }

    for (size_t i = 0; i < renderData_.immCmdBuffers.size(); i++) {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vk_.dispTable.beginCommandBuffer(renderData_.immCmdBuffers[i], &begin_info) != VK_SUCCESS) {
            ERR_LOG("Failed to begin command buffer: " + i);
            return VULKAN_ERROR;
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass           = renderData_.renderPass;
        renderPassInfo.framebuffer          = renderData_.framebuffers[i];
        renderPassInfo.renderArea.offset    = { 0, 0 };
        renderPassInfo.renderArea.extent    = vk_.swapchain.extent;
        VkClearValue clearColor{ { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        renderPassInfo.clearValueCount      = 1;
        renderPassInfo.pClearValues         = &clearColor;

        VkViewport viewport = {};
        viewport.x          = 0.0f;
        viewport.y          = 0.0f;
        viewport.width      = (float)vk_.swapchain.extent.width;
        viewport.height     = (float)vk_.swapchain.extent.height;
        viewport.minDepth   = 0.0f;
        viewport.maxDepth   = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = vk_.swapchain.extent;

        vk_.dispTable.cmdSetViewport(renderData_.immCmdBuffers[i], 0, 1, &viewport);
        vk_.dispTable.cmdSetScissor (renderData_.immCmdBuffers[i], 0, 1, &scissor);

        vk_.dispTable.cmdBeginRenderPass(renderData_.immCmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vk_.dispTable.cmdBindPipeline(renderData_.immCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, background_.pipeline);

        vk_.dispTable.cmdDraw(renderData_.immCmdBuffers[i], 3, 1, 0, 0);

        vk_.dispTable.cmdEndRenderPass(renderData_.immCmdBuffers[i]);

        if (vk_.dispTable.endCommandBuffer(renderData_.immCmdBuffers[i]) != VK_SUCCESS) {
            ERR_LOG("Failed to record command buffer: " + i);
            return VULKAN_ERROR;
        }
    }

    return SUCCESS;
}

Return_t Engine::init_material_() {

    return SUCCESS;
}

Return_t Engine::recreate_swapchain_() {
    MSG_LOG("Recreating swapchain...");

    vk_.dispTable.deviceWaitIdle();

    vk_.dispTable.destroyCommandPool(renderData_.immCmdPool, nullptr);

    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     vk_.dispTable.destroyFramebuffer(frames_[i].framebuffer, nullptr);
    // }
    for (auto framebuffer : renderData_.framebuffers) {
        vk_.dispTable.destroyFramebuffer(framebuffer, nullptr);
    }

    vk_.swapchain.destroy_image_views(renderData_.swapchainImageViews);

    if (!init_swapchain_())         return SWAPCHAIN_ERROR;
    if (!init_framebuffers_())      return SWAPCHAIN_ERROR;
    if (!init_command_pools_())     return SWAPCHAIN_ERROR;
    if (!init_command_buffers_())   return SWAPCHAIN_ERROR;

    return SUCCESS;
}

Return_t Engine::create_or_resize_buffer_(Buffer& buffer, size_t newSize) {
    if (buffer.handle != VK_NULL_HANDLE) {
        // vk_.dispTable.destroyBuffer(buffer.handle, nullptr);
        vmaDestroyBuffer(allocator_, buffer.handle, buffer.allocation);
    }
    if (buffer.memory != VK_NULL_HANDLE) {
        vk_.dispTable.freeMemory(buffer.memory, nullptr);
    }
    
    // Using VMA
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.size  = newSize;
    bufferInfo.usage = buffer.usage;

    VmaAllocationCreateInfo vmaAllocInfo = {};
    vmaAllocInfo.usage = buffer.vmaUsage;
    vmaAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

    // Allocate the buffer
    if (vmaCreateBuffer(
        allocator_, 
        &bufferInfo, 
        &vmaAllocInfo, 
        &buffer.handle, 
        &buffer.allocation,
        &buffer.info) != 
        VK_SUCCESS) {
        ERR_LOG("Failed to create a buffer");
        return BUFFER_ERROR;
    }

    return SUCCESS;
}

Return_t Engine::load_shader_(const std::filesystem::path& path, VkShaderModule* shaderModule) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        ERR_LOG(("Failed to load shader file: " + path.string()).c_str());
        return FILE_ERROR;
    }
    std::filesystem::path path2;

    // Get the file size
    stream.seekg(0, std::ios_base::end);
    std::streampos size = stream.tellg();
    stream.seekg(0, std::ios_base::beg);

    // Read the file
    std::vector<char> buffer(size);
    if (!stream.read(buffer.data(), size)) {
        ERR_LOG(("Could not read shader file: " + path.string()).c_str());
        return FILE_ERROR;
    }

    stream.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = (uint32_t*)buffer.data();
    createInfo.codeSize = buffer.size();

    if (vk_.dispTable.createShaderModule(&createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        ERR_LOG("Failed to load shader");
        return FILE_ERROR;
    }
    return SUCCESS;
}

void Engine::message_log_(const char* message, const char* file, int32_t line) {
	// printf("[ENGINE] MESSAGE in %s at line %i - \"%s\"\n\n", file, line, message);
    std::cout << "[ENGINE] Message in " << file << " at line " << line << " - \"" << message << "\"\n" << std::endl;
}

void Engine::error_log_(const char* message, const char* file, int32_t line) {
	// printf("[ENGINE] ERROR in %s at line %i - \"%s\"\n\n", file, line, message);
    std::cout << "[ENGINE] Error in " << file << " at line " << line << " - \"" << message << "\"\n" << std::endl;
}