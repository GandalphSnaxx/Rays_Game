#ifndef HELLO_TRIANGLE_HPP__
#define HELLO_TRIANGLE_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <optional>
#include <map>
#include <set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <array>
#include <chrono>

// #include "vulkan/vk_handler.hpp"
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
    void run() {
        // vulkanHandler.init(&WINDOW);

// #ifdef PRINT_EXTENSIONS
//         printExtensions();
// #endif
//         mainLoop();
//         cleanup();
    }

    private:

    // Private member constants
    // Window defaults
    const VkOverheadClass::WindowInit _WINDOW = {
        800,                // WIDTH
        600,                // HEIGHT
        "Hello Triangle",   // TITLE
        nullptr,            // MONITOR
        nullptr             // SHARE
    };
    // Application name
    const char *_APP_NAME = "Hello 3D Rendering!";

    // Define frame buffer size
    const uint32_t _MAX_FRAMES_IN_FLIGHT = 2;

    // Private member variables
    VkHandlerClass _vulkanHandler{&_WINDOW, _APP_NAME, _MAX_FRAMES_IN_FLIGHT};

    // Private member functions
    // void initWindow();
    // void initVulkan();
    // void mainLoop();
    // void cleanup();
};

#endif