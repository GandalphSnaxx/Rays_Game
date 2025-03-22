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
#include <iostream>

#ifdef _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define MSG_LOG(m) message_log_(m, __FILENAME__, __LINE__)
#define ERR_LOG(m) error_log_(m, __FILENAME__, __LINE__)

using namespace vk;

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Public Member Functions
//---------------------------------------------------------------------------------------------------------------------------//

Return_t Engine::init(const char* title) {
    flags_ += VALIDATION_LAYERS; // Add Vulkan validation layers

    // Initalize each part
    if (!init_sdl_(title)) { 
        ERR_LOG("init_sdl_ failed");
        return false; 
    }
    if (!init_vulkan_(title)) { 
        ERR_LOG("init_vulkan_ failed");
        return false; 
    }
    if (!pick_physical_device_()) { 
        ERR_LOG("pick_physical_device_ failed");
        return false; 
    }
    if (!create_device_()) { 
        ERR_LOG("create_device_ failed");
        return false; 
    }
    if (!create_swapchain_()) { 
        ERR_LOG("create_swapchain_ failed");
        return false; 
    }
    if (!create_cmd_pool_()) { 
        ERR_LOG("create_cmd_pool_ failed");
        return false; 
    }


    flags_ += INITALIZED; // Everything went correctly
    return true;
}

Return_t Engine::draw() {

}

Return_t Engine::clean() {
    if (flags_(CLEANED)) {
        MSG_LOG("Tried to clean a cleaned engine");
        return true; 
    }
    if (!flags_(INITALIZED)) {
        MSG_LOG("Tried to clean an uninitalized engine");
        return true;
    }

    deleteQueue_.flush(); // Call everything in the deletion queue

    flags_ += CLEANED; // Everything went correctly
    return SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Private Member Functions
//---------------------------------------------------------------------------------------------------------------------------//

Return_t Engine::init_sdl_(const char* title, const bool fullscreen /* = false */, const bool borderless /* = false */) {
    MSG_LOG("Initlalizing SDL...");

    // We initialize SDL and create a window with it.
    if(!SDL_Init(SDL_INIT_VIDEO)) {
		ERR_LOG("Failed to initialize SDL");
		return false;
	}

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    window_flags |= (SDL_WindowFlags)(SDL_WINDOW_FULLSCREEN * fullscreen); // Maybe set later?
    window_flags |= (SDL_WindowFlags)(SDL_WINDOW_BORDERLESS * fullscreen); // Maybe set later?

    window_ = SDL_CreateWindow(
        title,
        // SDL_WINDOWPOS_UNDEFINED,
        // SDL_WINDOWPOS_UNDEFINED,
        windowExtent_.width,
        windowExtent_.height,
        window_flags);

    if (window_ == nullptr) {
        ERR_LOG("Failed to create a window");
        return false;
    }

    // Add window destruction to the deletion queue
    deleteQueue_.add([&]() {
        SDL_DestroyWindow(window_);
    });

    return true; // Success
}

Return_t Engine::init_vulkan_(const char* appName) {
    // Use VkBootstrap to initalize Vulkan
    vkb::InstanceBuilder builder;
    auto inst_ret = builder
        .set_app_name(appName)
        .request_validation_layers()
        .use_default_debug_messenger()
        .build();

    if (!inst_ret) { /* report */ }
    vkb::Instance vkb_inst = inst_ret.value ();

    vkb::PhysicalDeviceSelector selector{ vkb_inst };
    auto phys_ret = selector.set_surface (surface)
                        .set_minimum_version (1, 1)
                        .require_dedicated_transfer_queue ()
                        .select ();
    if (!phys_ret) { /* report */ }

    vkb::DeviceBuilder device_builder{ phys_ret.value () };
    auto dev_ret = device_builder.build ();
    if (!dev_ret) { /* report */ }
    vkb::Device vkb_device = dev_ret.value ();

    auto graphics_queue_ret = vkb_device.get_queue (vkb::QueueType::graphics);
    if (!graphics_queue_ret)  { /* report */ }
    VkQueue graphics_queue = graphics_queue_ret.value ();

    return SUCCESS;
}

Return_t Engine::pick_physical_device_() {
    
    return SUCCESS;
}

Return_t Engine::create_device_() {
    
    return SUCCESS;
}

Return_t Engine::create_swapchain_() {
    
    return SUCCESS;
}

Return_t Engine::create_cmd_pool_() {
    
    return SUCCESS;
}

void Engine::message_log_(const char* message, const char* file, int32_t line) {
	printf("[ENGINE] MESSAGE in %s at line %i - \"%s\"\n\n", file, line, message);
}

void Engine::error_log_(const char* message, const char* file, int32_t line) {
	printf("[ENGINE] ERROR in %s at line %i - \"%s\"\n\n", file, line, message);
}