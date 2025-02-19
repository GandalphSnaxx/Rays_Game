#ifndef VULKAN_OVERHEAD_HPP__
#define VULKAN_OVERHEAD_HPP__


#include "vulkan_config.hpp"
#include "raysDebugHelper.hpp"

// #include <array>
// #include <vector>

/// @brief A class that other vulkan classes can be children of. This class holds variables and functions that multiple child classes require
class VkOverheadClass {
    public:
    VkOverheadClass();
    ~VkOverheadClass();

    protected:

    // Device related variables
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
    VkDevice _device;

    // Instance related variables
    VkInstance _instance;

    // Surface related variables
    VkSurfaceKHR _surface;

    // Window related variables
    GLFWwindow *_window;

    private:

};

#endif