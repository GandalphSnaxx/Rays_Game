/**
 * @file vk_engine.hpp
 * @author Ray Richter
 * @brief Vulkan engine
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef VK_ENGINE_HPP
#define VK_ENGINE_HPP

#include <FlagField.hpp>

#include "vk_types.hpp"
#include "deleteQueue.hpp"
namespace vk {

class Engine {
public:

    ~Engine() { if(!flags_(CLEANED)) { clean(); } }

    Return_t init(const char* title);
    Return_t draw();
    Return_t clean();

    Return_t fullscreen();
    Return_t fullscreen_borderless();
    Return_t resize();
    Return_t minimize();

private:
    typedef enum EngineStateFlags_ {
        INITALIZED,
        CLEANED,
        FULLSCREEN, FULLSCREEN_REQUESTED,
        BORDERLESS, BORDERLESS_REQUESTED,
        MINIMIZED,  MINIMIZE_REQUESTED,
        RESIZE_REQUESTED,
        SHUTDOWN_REQUESTED,
        VALIDATION_LAYERS,
        MAX
    } EngineStateFlags_;
    FlagField<MAX, EngineStateFlags_> flags_;

    DeletionQueue deleteQueue_;

    // GLFWwindow* window_;
    // struct SDL_Window* window_ { nullptr };
	// VkExtent2D windowExtent_ = { 800, 600 };

    VkData vk = { .extent = {800, 600} }; // Vulkan data

    Return_t init_sdl_(const char* title, const bool fullscreen = false, const bool borderless = false);
    Return_t init_vulkan_(const char* appName);
    Return_t pick_physical_device_();
    Return_t create_device_();
    Return_t create_swapchain_();
    Return_t create_cmd_pool_();

    static void message_log_(const char* message, const char* file, int32_t line);
    static void error_log_  (const char* message, const char* file, int32_t line);
};

} // namespace vk

#endif // VK_ENGINE_HPP