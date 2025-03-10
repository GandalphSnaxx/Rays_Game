#pragma once
/**
 * @file hello_triangle.hpp
 * @author Ray Richter
 * @brief My hello triangle application that uses my Vulkan engine.
 */

// #include <Validator.hpp>
#include <Validator.hpp>
#include "hello_triangle_config.hpp"
// #define EN_VALIDATION_LAYERS
#include "vulkan/handler.hpp"
#include "raysDebugHelper.hpp"

// Print the extensions available when uncommented
// #define PRINT_EXTENSIONS

/// @brief A class for holding the Hello Triangle application
class HelloTriangleApplication {
    public:

    // Constructor and deconstructor
    HelloTriangleApplication();
    ~HelloTriangleApplication();

    // Public member functions
    void run();

    private:

    // Private member constants
    // Window defaults
    const WindowInit WINDOW_ = {
        800,                // WIDTH
        600,                // HEIGHT
        "Hello Triangle",   // TITLE
        nullptr,            // MONITOR
        nullptr             // SHARE
    };
    // Application name
    const char *APP_NAME_ = "Hello 3D Rendering!";

    // Define frame buffer size
    const uint32_t MAX_FRAMES_IN_FLIGHT_ = 2;

    // Vulkan handler with constructor arguments
    VkHandler vulkanHandler_ = {WINDOW_, APP_NAME_, MAX_FRAMES_IN_FLIGHT_, VERTICES, INDICES};

    // Private member functions
    // void initWindow();
    // void initVulkan();
    // void mainLoop();
    // void cleanup();
};