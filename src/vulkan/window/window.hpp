#ifndef VULKAN_WINDOW_HPP__
#define VULKAN_WINDOW_HPP__

#include "overhead.hpp"

/// @brief A class containing window related variables and functions. VkWindowClass is a child of VkOverheadClass.
///     Requires access to _window
class VkWindowClass : virtual public VkOverheadClass {
    public:

    VkWindowClass(const WindowInit *window_init);
    ~VkWindowClass();
    // Delete the default constructor to prevent its use
    VkWindowClass::VkWindowClass() = delete;

    protected:

    private:

    // Private member variables
    bool _framebufferResized = false;

    // Private member functions
    static void _framebufferResizeCallback(GLFWwindow *window, int width, int height);

    // const WindowInit _badWindow = {800, 600, "BAD WINDOW"};
};

#endif