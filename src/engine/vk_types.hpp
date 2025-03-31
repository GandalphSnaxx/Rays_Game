/**
 * @file vk_types.hpp
 * @author Ray Richter
 * @brief Vulkan includes and types.
 * @version 0.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef VK_TYPES_HPP
#define VK_TYPES_HPP

#include <array>
#include <fstream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrap.h>
// #define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include "../utils/deleteQueue.hpp"

/// @brief Vulkan initalizer data.
struct VkInit {
    const char* appName;
    VkExtent2D  defaultWindowSize;
    bool validationLayersEnable = false;
};

/// @brief Vulkan data.
struct VkData {
    SDL_Window*                 window;
    VkExtent2D                  extent;
    vkb::Instance               instance;
    vkb::InstanceDispatchTable  instDispTable;
    VkSurfaceKHR                surface;
    vkb::Device                 device;
    vkb::DispatchTable          dispTable;
    vkb::Swapchain              swapchain;
};

struct RenderData {
    VkQueue                     graphicsQueue;
    VkQueue                     presentQueue;

    std::vector<VkImage>        swapchainImages;
    std::vector<VkImageView>    swapchainImageViews;
    std::vector<VkFramebuffer>  framebuffers;

    VkRenderPass                renderPass;
    VkPipelineLayout            pipelineLayout;
    VkPipeline                  graphicsPipeline;

    VkCommandPool               immCmdPool;
    std::vector<VkCommandBuffer>immCmdBuffers;
    VkFence                     immFence;

    std::vector<VkSemaphore>    availableSemaphores;
    std::vector<VkSemaphore>    finishedSemaphore;
    std::vector<VkFence>        inFlightFences;
    std::vector<VkFence>        imageInFlight;

    size_t                      currentFrame = 0;
};

struct FrameData {
    VkSemaphore     swapchainSemaphore, 
                    renderSemaphore;
    VkFence         renderFence;

    VkCommandPool   cmdPool;
    VkCommandBuffer cmdBuffer;

    DeletionQueue   deleteQueue;
    // Descriptor   descriptors;
};

struct EngineStats {
    size_t frametime_us;
    size_t meshDrawTime_us;
    size_t triangleCount;
    size_t drawcallCount;
};

struct AllocatedImage {
    VkImage         image;
    VkImageView     imageView;
    VmaAllocation   allocation;
    VkExtent3D      extent;
    VkFormat        imageFormat;
};

struct ShaderFile {
    std::string path;
    VkShaderStageFlagBits type;
};

struct Buffer {
    VkBuffer                handle      = nullptr;
    VkDeviceMemory          memory      = nullptr;
    VkDeviceSize            size        = 0;
    VkBufferUsageFlagBits   usage       = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
    VmaMemoryUsage          vmaUsage    = VMA_MEMORY_USAGE_MAX_ENUM;
    VmaAllocation           allocation  = nullptr;
    VmaAllocationInfo       info        = {};
};

#endif // VK_TYPES_HPP
