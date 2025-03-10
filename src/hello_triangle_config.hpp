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
#include "vulkan/vertex/Vertex.hpp"
#include "vulkan/window/WindowInit.hpp"

/// @section Common Config

#define APP_NAME "Hello Ray's Vulkan Engine"
#define MIN_API_VERSION 0

const WindowInit WINDOW_ = {
    (int)800,           // WIDTH
    (int)600,           // HEIGHT
    "Hello Triangle",   // TITLE
    nullptr,            // MONITOR
    nullptr             // SHARE
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> VALIDATION_LAYERS = {};
//     const std::vector<const char*> VALIDATION_LAYERS = {
//     "VK_LAYER_KHRONOS_validation"
// };

// // Example triangle verticies
// const std::vector<Vertex> triangleVertices = {
//     // All values are 0.0f to 1.0f
//     // {{pos x, pos y}, {r, g, b}}
//     {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
//     {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
//     {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
// };

// A description of the types of resources to be accessed by the pipeline
// struct UniformBufferObject {
//     glm::mat4 model;
//     glm::mat4 view;
//     glm::mat4 proj;
// };

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