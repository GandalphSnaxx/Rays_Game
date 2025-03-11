#include "hello_triangle.hpp"

/// @section Constructors

HelloTriangleApplication::HelloTriangleApplication() {
    DEBUG_MSG("Called HelloTriangleApplication constructor");
}

HelloTriangleApplication::~HelloTriangleApplication() {
    DEBUG_MSG("Called HelloTriangleApplication deconstructor");
    vulkanHandler_.~VkHandler();
}

/// @section Public Member Functions

void HelloTriangleApplication::run() {
    // Do nothing
    while (!vulkanHandler_) {
        vulkanHandler_.mainLoop(); // Call the vulkan handler main function

        // Do cpu functions here
    }
    vulkanHandler_.endLoop();
}

/// @section Private Member Functions

