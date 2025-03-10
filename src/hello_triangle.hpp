#pragma once
/**
 * @file hello_triangle.hpp
 * @author Ray Richter
 * @brief My hello triangle application that uses my Vulkan engine.
 */
#define USE_64_BIT_MODE
#include <Validator.hpp>
#define EN_VALIDATION_LAYERS
#include "vulkan/handler.hpp"
#include "hello_triangle_config.hpp"
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
    // Vulkan handler with constructor arguments
    VkHandler vulkanHandler_ = {WINDOW_PARAMS, APP_NAME, MAX_FRAMES_IN_FLIGHT, VERTICES, INDICES};

    // Private member functions
    // void initWindow();
    // void initVulkan();
    // void mainLoop();
    // void cleanup();
};