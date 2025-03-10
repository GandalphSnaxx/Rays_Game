#pragma once
/**
 * @file hello_triangle_config.hpp
 * @author Ray Richter
 * @brief Configuration definitions for use with my hello triangle application.
 */
#include <vector>
#include "vulkan/vertex/Vertex.hpp"
#include "vulkan/window/WindowInit.hpp"

/// @section Common Configuration Data

// Inital window parameters
constexpr WindowInit WINDOW_PARAMS = {
    (int)800,           // WIDTH
    (int)600,           // HEIGHT
    "Hello Triangle",   // TITLE
    nullptr,            // MONITOR
    nullptr             // SHARE
};

// Application name
constexpr char *APP_NAME = "Hello 3D Rendering!";

// Define frame buffer size
constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

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