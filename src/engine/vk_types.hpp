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

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

#define MAX_FRAMES_IN_FLIGHT 2

/// @brief Labeled return data. Errors are negative, success are positive.
typedef enum Return_t {
    SUCCESS         =  0,   // Success   
    SOFT_SUCCESS    =  1,   // Something unexpected occurred

    ERROR           = -1,   // Error
} Return_t;

/// @brief Vulkan initalizer data.
struct VkInit {
    const char* app_name;
    VkExtent2D  defaultWindowSize;
    bool validationLayersEnable = false;
};

/// @brief Vulkan data.
struct VkData {
    struct SDLWindow*           window;
    VkExtent2D                  extent;
    vkb::Instance               instance;
    vkb::InstanceDispatchTable  inst_disp;
    VkSurfaceKHR                surface;
    vkb::Device                 device;
    vkb::DispatchTable          disp;
    vkb::Swapchain              swapchain;
};

#endif // VK_TYPES_HPP
