#ifndef HELLO_TRIANGLE_HPP__
#define HELLO_TRIANGLE_HPP__



// #include "vulkan/vk_handler.hpp"
#include "vulkan/handler.hpp"
#include "raysDebugHelper.hpp"

// Print the extensions available when uncommented
// #define PRINT_EXTENSIONS

// Example triangle verticies
const std::vector<Vertex> triangleVertices = {
    // All values are 0.0f to 1.0f
    // {{pos x, pos y}, {r, g, b}}
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

// Example rectangle verticies
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0
};

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