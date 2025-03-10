#pragma once
/**
 * @file
 * @author Ray Richter
 * @brief A struct for holding shader file information.
 */
#include <vulkan/vulkan.h>
#include <fstream>
#include <string>
#include <vector>
#include "raysDebugHelper.hpp"

struct ShaderFile { public:
    /// @brief Shader file constructor.
    /// @param path Path and name to a .spv shader file.
    /// @param flags Shader stage flags marking the shader type.
    /// @param entryPoint Operator entry point. Default = `main`.
    ShaderFile(const char* path, const VkShaderStageFlagBits flags, const char* entryPoint = "main") : 
        fileName(path), opEntryPoint(entryPoint), shaderType(flags) {}

    const char* fileName;
    const VkShaderStageFlagBits shaderType;
    const char* opEntryPoint;

    /// @brief Read this shader file and return its raw binary
    /// @return The file's raw binary
    const std::vector<char> getBinary() const {
        // Open a file at the end so we can get its size
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        // Make sure the file opened correctly
        if (!file.is_open()) { THROW_ERR("failed to open file!"); }

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