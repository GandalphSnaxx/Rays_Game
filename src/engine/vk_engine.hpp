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
#include <filesystem>

#include "vk_types.hpp"
#include "camera.hpp"
#include "../utils/deleteQueue.hpp"
#include "materials.hpp"
#include "constants.hpp"
namespace vk {

class Engine {
public:

    ~Engine() { if(!flags_(CLEANED)) { clean(); } }

    Return_t init(const VkInit& init = {});
    Return_t draw(const SDL_Event& event);
    Return_t clean();
    bool should_close() const { return flags_(SHUTDOWN_REQUESTED); }

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

    DeletionQueue   deleteQueue_;
    VkInit          init_;
    VkData          vk_ = { .extent = {800, 600} }; // Vulkan data
    VmaAllocator    allocator_;
    RenderData      renderData_;
    EngineStats     stats_ = {};
    // FrameData       frames_[MAX_FRAMES_IN_FLIGHT];  // Data attached to each frame
    Background      background_;
    // MaterialPipeline    background_;    // Background compute shader
    Object          triangle_;      // Hello triangle vertex and fragment shaders
    // Buffer              triangleBuffer_;

    // Swapchain swapchain_;
    // SwapchainImages swapchainImages_;
    // Framebuffer framebuffer;
    // RenderPass renderPass_;
    // Pipeline pipeline_;

    // Private Initalizers
    // ----
    Return_t init_sdl_();
    Return_t init_vulkan_();
    Return_t init_vma_();
    Return_t init_swapchain_();
    Return_t init_queues_();
    Return_t init_render_pass_();
    Return_t init_pipelines_();
    Return_t init_framebuffers_();
    Return_t init_command_pools_();
    Return_t init_command_buffers_();
    Return_t init_sync_();
    // Return_t init_descriptors_();
    // Return_t init_default_data_();
    // Return_t init_renderables_();
    // Return_t init_imgui_();

    // Private Helper Functions
    // ----
    Return_t create_surface_sdl_();
    Return_t create_swapchain_();
    Return_t init_background_pipeline_();
    Return_t init_triangle_pipeline_();
    Return_t init_frames_();
    Return_t init_triangle_data_();
    Return_t create_cmd_pool_();
    Return_t create_cmd_buffers_();
    Return_t init_triangle_vertex_buffers_();
    Return_t init_material_();
    Return_t recreate_swapchain_();

    Return_t create_or_resize_buffer_(Buffer& buffer, size_t newSize);
    Return_t load_shader_(const std::filesystem::path& path, VkShaderModule* shaderModule);

    static void message_log_(const char* message, const char* file, int32_t line);
    static void error_log_  (const char* message, const char* file, int32_t line);
};

} // namespace vk

#endif // VK_ENGINE_HPP