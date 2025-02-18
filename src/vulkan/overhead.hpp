#ifndef VULKAN_OVERHEAD_HPP__
#define VULKAN_OVERHEAD_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <vector>

#include "raysDebugHelper.hpp"

/// @brief A structure for storing shader files and their shader type
struct ShaderFile {
    const std::string fileName;
    const VkShaderStageFlagBits shaderType;
};
// Shader file names, locations, and types
const std::vector<ShaderFile> SHADER_FILES = {
    {"src/shaders/fragment/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT},
    {"src/shaders/vertex/vert.spv", VK_SHADER_STAGE_VERTEX_BIT}
};

/// @brief A class that other vulkan classes can be children of. This class holds variables and functions that multiple child classes require
class VkOverheadClass {
    public:

    /// @brief A structure for initializing a window
    /// @param width Initial window width
    /// @param height Initial window height
    /// @param title Initial window title, UTF-8 encoding
    /// @param monitor Fullscreen monitor, or NULL for windowed
    /// @param share A window to share resources with, or NULL
    struct WindowInit {
        int width;
        int height;
        const char *title;
        GLFWmonitor *monitor = nullptr;
        GLFWwindow *share = nullptr;
    };

    // Constructor and deconstructor
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