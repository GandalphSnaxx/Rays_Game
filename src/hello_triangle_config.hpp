#pragma once
/**
 * @file hello_triangle_config.hpp
 * @author Ray Richter
 * @brief Configuration definitions for use with my hello triangle application.
 */

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include "vertex/Vertex.hpp"

/// @section Common Config

#define APP_NAME "Hello Ray's Vulkan Engine"
#define MIN_API_VERSION 0

const WindowInit WINDOW_ = {
    800,                // WIDTH
    600,                // HEIGHT
    "Hello Triangle",   // TITLE
    nullptr,            // MONITOR
    nullptr             // SHARE
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    /// @brief A member function for getting the binding description of this vertex
    /// @return Vulkan vertex input binding description struct
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    /// @brief A member function for getting attribute descriptions
    /// @return 2 vulkan vertex input attribute descriptions
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    
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

// Example triangle verticies
const std::vector<Vertex> triangleVertices = {
    // All values are 0.0f to 1.0f
    // {{pos x, pos y}, {r, g, b}}
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

// A description of the types of resources to be accessed by the pipeline
struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

// Example rectangle verticies and indices
const std::vector<Vertex> VERTICES = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
const std::vector<uint32_t> INDICES = {
    0, 1, 2, 2, 3, 0
};