#ifndef VULKAN_HANDLER__
#define VULKAN_HANDLER__
// This file is the header for any function that handles Vulkan commands after initialization

#include "descriptors/vk_descriptors.hpp"
#include "devices/vk_devices.hpp"
#include "instance/vk_instance.hpp"
#include "vulkan/graphics_pipeline/vk_graphics_pipeline.hpp"
#include "render_pass/vk_render_pass.hpp"
#include "surface/vk_surface.hpp"
#include "window/vk_window.hpp"


/// @brief A structure for handling Vulkan commands
class VkHandler {
    public:

    // Constructor and deconstructor
    VkHandler();
    ~VkHandler();

    VkResult init(const WindowInit *windowParams);

    private:

    // Private member variables
    VkWindowClass window;
    VkInstanceClass instance;
    VkSurfaceClass surface;
    VkDevicesClass devices;
    VkSwapchainClass swapchain;
    VkRenderPassClass renderPass;
    VkDescriptorClass descriptors;
    VkPipelineClass gfxPipeline;
};

#endif