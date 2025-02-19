/* ******************************************************** *
 * Various structs for organizing Vulkan configuration data *
 * ******************************************************** *
 * Ray Richter                                              *
 * ******************************************************** */

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

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "raysDebugHelper.hpp"

/// @brief A structure for storing shader files and their shader type
struct ShaderFile {
    const std::string fileName;
    const VkShaderStageFlagBits shaderType;

    /// @brief Read this shader file and return its raw binary
    /// @return The file's raw binary
    const std::vector<char> getBinary() const {
        // Open a file at the end so we can get its size
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        // Make sure the file opened correctly
        if (!file.is_open()) {
            THROW_ERR("failed to open file!");
        }

        // Get file size
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        // Goto the beginning of the file and read all of the bytes at once
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        // Check if we are reading the files correctly
        DEBUG_VAR(fileSize);

        // Close the file and return the read data
        file.close();
        return buffer;
    }
};

/// @brief A structure for storing dynamic states for pipeline creation
struct DynamicStateList {
    uint32_t enabledStateBytefield;
    std::vector<VkDynamicState> list;
};

/// @brief A struct for storing vertex information and member functions
struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    // A member function for getting the binding description
    static std::vector<VkVertexInputBindingDescription> getBindingDescription() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    // A member function for getting attribute descriptions
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

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