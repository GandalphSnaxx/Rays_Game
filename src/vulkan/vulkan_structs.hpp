#pragma once
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

/// @brief A description of the types of resources to be accessed by the pipeline
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};