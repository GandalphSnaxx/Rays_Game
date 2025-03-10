#pragma once
/**
 * @file vulkan/handler.hpp
 * @author Ray Richter
 * @brief VkHandler class declaration.
 */

#include "window/WindowManager.hpp"
#include "instance/VulkanInstance.hpp"
#include "surface/SurfaceManager.hpp"
#include "devices/DeviceManager.hpp"
#include "swapchain/SwapchainManager.hpp"
#include "render_pass/RenderPass.hpp"
#include "descriptors/DescriptorLayoutManager.hpp"
#include "pipeline/PipelineManager.hpp"
#include "buffers/FrameBufferManager.hpp"
#include "buffers/CommandPoolManager.hpp"
#include "buffers/BufferManager.hpp"
#include "buffers/UniformBufferManager.hpp"
#include "descriptors/DescriptorManager.hpp"
#include "syncronization/SyncManager.hpp"

/// @section Vulkan Handler Class Definition

/// @brief A class handling data and functions for managing Vulkan
class VkHandler {
public:
/// @section Vulkan Handler Constructors

    /// @brief Initalizes a Vulkan Handler with a constructor.
    /// @param windowInit Initial window parameters as `const WindowInit&`.
    /// @param appName Application name as UTF-8 `const char*`.
    /// @param maxFramesInFlight Maximum number of frames in flight as `const uint32_t&`.
    /// @param vertices Vertex information list.
    /// @param indices Index information list.
    VkHandler(
        const WindowInit &windowInit, 
        const char* appName, 
        const uint32_t &maxFramesInFlight, 
        const std::vector<Vertex> &vertices,
        const std::vector<uint32_t> &indices);
    /// @brief Default Vulkan Handler constructor. Init function still needs to be called.
    VkHandler();
    /// @brief Vulkan Handler deconstructor. Cleans up any data used.
    ~VkHandler();

/// @section Public Member Functions

    /// @brief Initializes a Vulkan Handler with a function.
    /// @param windowInit Initial window parameters.
    /// @param appName Application name as UTF-8 `char*`.
    /// @param maxFramesInFlight Maximum frames in flight.
    /// @return `VkResult`
    VkResult init(
        const WindowInit &windowInit, 
        const char* appName, 
        const uint32_t &maxFramesInFlight, 
        const std::vector<Vertex> &vertices,
        const std::vector<uint32_t> &indices);
    
    void mainLoop();
    void drawFrame();
    operator  bool() const { return glfwWindowShouldClose(window_.getWindow()) == 0; }
    /// @brief Checks if the window should not close
    /// @return `FALSE` if the window should close, `TRUE` if the window does not need to close.
    bool operator!() const { return glfwWindowShouldClose(window_.getWindow()) != 0; }

private:
/// @section Vulkan Handler Internal Classes
    WindowManager           window_;
    VulkanInstance          instance_;
    SurfaceManager          surface_;
    DeviceManager           device_;
    SwapchainManager        swapchain_;
    RenderPassManager       renderPass_;
    DescriptorLayoutManager descLayout_;
    PipelineManager         pipeline_;
    FrameBufferManager      framebuffers_;
    CommandPoolManager      cmdPool_;
    BufferManager<Vertex>   vertexBuffer_;
    BufferManager<uint32_t> indexBuffer_;
    UniformBufferManager    uboManager_;
    DescriptorManager       descriptors_;
    SyncManager             sync_;
    uint32_t                currentFrame_;
    size_t                  maxFIF_;

/// @section Vulkan Configuration Constants
#ifdef EN_VALIDATION_LAYERS
    const std::vector<const char*> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation" };
    const bool VALIDATION_LAYERS_EN = true;
#else
    const std::vector<const char*> VALIDATION_LAYERS = {};
    const bool VALIDATION_LAYERS_EN = false;
#endif

    VkResult init_(
        const WindowInit&, 
        const char*, 
        const uint32_t&, 
        const std::vector<Vertex>&, 
        const std::vector<uint32_t>&); // Internal init function
    VkResult recordCommandBuffer(const uint32_t &currentFrame, const uint32_t &imageIndex);
};