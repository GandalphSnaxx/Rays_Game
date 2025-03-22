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
#define ERR_LOG(m) message_log_(m, __FILENAME__, __LINE__)

using namespace vk;

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Public Member Functions
//---------------------------------------------------------------------------------------------------------------------------//

bool Engine::init() {
    // Initalize each part
    if(!init_glfw_()            ) return false;
    if(!create_instance_()      ) return false;
    if(!pick_physical_device_() ) return false;
    if(!create_device_()        ) return false;
    if(!create_swapchain_()     ) return false;
    if(!create_cmd_pool_()      ) return false;


    flags_ += INITALIZED; // Everything went correctly
    return true;
}

bool Engine::draw() {

}

bool Engine::clean() {
    if(flags_(CLEANED)) return; // Already cleaned

    deleteQueue_.flush(); // Call everything in the deletion queue

    flags_ += CLEANED; // Everything went correctly
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------//
/// @section Private Member Functions
//---------------------------------------------------------------------------------------------------------------------------//

bool Engine::init_glfw_() {
    MSG_LOG("initlalizing GLFW...");

	if(!glfwInit()) {
		ERR_LOG("failed to initialize GLFW");
		return VKH_FALSE;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	inst->window = glfwCreateWindow(w, h, name, NULL, NULL);
	if(!inst->window) {
		ERR_LOG("failed to create GLFW window");
		return false;
	}

    return true; // Success
}

bool Engine::create_instance_() {
    
    return true; // Success
}

bool Engine::pick_physical_device_() {
    
    return true; // Success
}

bool Engine::create_device_() {
    
    return true; // Success
}

bool Engine::create_swapchain_() {
    
    return true; // Success
}

bool Engine::create_cmd_pool_() {
    
    return true; // Success
}

void Engine::message_log_(const char* message, const char* file, int32_t line) {
	printf("[ENGINE] MESSAGE in %s at line %i - \"%s\"\n\n", file, line, message);
}

void Engine::error_log_(const char* message, const char* file, int32_t line) {
	printf("[ENGINE] ERROR in %s at line %i - \"%s\"\n\n", file, line, message);
}