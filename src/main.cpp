#ifndef DEBUG
#define DEBUG
#endif

// Print the vulkan extensions available when uncommented
// #define PRINT_EXTENSIONS

#include "hello_triangle.hpp"

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}