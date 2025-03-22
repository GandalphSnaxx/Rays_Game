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

    bool init();
    bool draw();
    bool clean();

private:
    typedef enum EngineStateFlags_ {
        INITALIZED,
        CLEANED,
        MINIMIZED,
        RESIZE_REQUESTED,
        SHUTDOWN_REQUESTED,
        MAX
    } EngineStateFlags_;
    FlagField<MAX, EngineStateFlags_> flags_;

    DeletionQueue deleteQueue_;

    GLFWwindow* window_;

    bool init_glfw_();
    bool create_instance_();
    bool pick_physical_device_();
    bool create_device_();
    bool create_swapchain_();
    bool create_cmd_pool_();

    static void message_log_(const char* message, const char* file, int32_t line);
    static void error_log_  (const char* message, const char* file, int32_t line);
};

} // namespace vk

#endif // VK_ENGINE_HPP