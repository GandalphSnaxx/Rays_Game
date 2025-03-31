/**
 * @file constants.hpp
 * @author Ray Richter
 * @brief Engine constants
 * @version 0.1
 * @date 2025-03-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef VK_ENGINE_CONSTANTS_HPP
#define VK_ENGINE_CONSTANTS_HPP

#include "vk_types.hpp"

constexpr int               MAX_FRAMES_IN_FLIGHT = 2;
constexpr std::string_view  SHADER_DIRECTORY = "src/shaders/bin";

/// @brief Labeled return data. Errors are negative, success are positive.
typedef enum Return_t : int {
    SUCCESS             =  0,   // Success   
    SOFT_SUCCESS        =  1,   // Something unexpected occurred
    DONE_ALREADY        =  2,   // Task already completed
    UNNECESSARY         =  3,   // Function was called unnecessarily

    ERROR               = -1,   // Generic errors
    SDL_ERROR           = -2,   // SDL errors
    VULKAN_ERROR        = -3,   // Vulkan errors
    COMMAND_ERROR       = -4,   // Command errors
    SYNC_ERROR          = -5,   // Sync errors
    DESCRIPTOR_ERROR    = -6,   // Descriptor errors
    PIPELINE_ERROR      = -7,   // Pipeline errors
    DEFAULT_DATA_ERROR  = -8,   // Data initalizer errors
    RENDERABLES_ERROR   = -9,   // Renderables errors
    IMGUI_ERROR         = -10,  // IMGUI errors
    VK_BOOTSTRAP_ERROR  = -11,  // Vulkan bootstrap errors
    BUFFER_ERROR        = -12,  // Buffer errors
    FILE_ERROR          = -13,  // Filesystem errors
    SWAPCHAIN_ERROR     = -14,  // Swapchain errors
    RENDER_PASS_ERROR   = -15,  // Render pass errors
    FRAME_ERROR         = -16,  // Frame errors
} Return_t;

// Window flags for window creation with SDL
constexpr SDL_WindowFlags   WINDOW_FLAGS = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

// Vma allocator info constants
constexpr VmaAllocatorCreateInfo ALLOC_INFO = { .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT };

// Command pool constants
// ----
// Create info constants
constexpr VkCommandPoolCreateInfo POOL_INFO = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
};
// Command buffer allocator info constants
constexpr VkCommandBufferAllocateInfo CMD_ALLOC_INFO = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = nullptr,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1,
};

// Sync structure constants
// ----
constexpr VkSemaphoreCreateInfo SEMAPHORE_INFO = { .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
constexpr VkFenceCreateInfo FENCE_INFO = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
};

#endif // VK_ENGINE_CONSTANTS_HPP