#include "structs.hpp"

/// @brief Initalizes a rasterizer state for pipeline creation
/// @param polyMode Determines how fragments are generated for geometry. The following modes are available:
///
/// - VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
///
/// - VK_POLYGON_MODE_LINE: polygon edges are drawn as lines
///
/// - VK_POLYGON_MODE_POINT: polygon vertices are drawn as points
RasterizerState::RasterizerState(const VkPolygonMode polyMode) {
    RasterizerState(polyMode, 1.0f, VK_FALSE);
}

/// @brief Initalizes a rasterizer state for pipeline creation
/// @param polyMode Determines how fragments are generated for geometry. The following modes are available:
///
/// - VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
///
/// - VK_POLYGON_MODE_LINE: polygon edges are drawn as lines
///
/// - VK_POLYGON_MODE_POINT: polygon vertices are drawn as points
///
/// . @param lineWidth Line width in terms of number of fragments
RasterizerState::RasterizerState(const VkPolygonMode polyMode, const float lineWidth) {
    RasterizerState(polyMode, lineWidth, VK_FALSE);
}

/// @brief Initalizes a rasterizer state for pipeline creation
/// @param polyMode Determines how fragments are generated for geometry. The following modes are available:
///
/// - VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
///
/// - VK_POLYGON_MODE_LINE: polygon edges are drawn as lines
///
/// - VK_POLYGON_MODE_POINT: polygon vertices are drawn as points
///
/// . @param lineWidth Line width in terms of number of fragments
/// @param depthClampEnable Not supported yet...
RasterizerState::RasterizerState(const VkPolygonMode polyMode, const float lineWidth, const VkBool32 depthClampEnable) {
    DEBUG_MSG("Called a RasterizerState struct constructor");
    /// TODO: check for lineWidth > 1.0f gpu support
    if (lineWidth > 1.0f) { THROW_ERR("Line width > 1.0 requires a gpu feature!"); }
    /// TODO: Add depth buffer support
    if (depthClampEnable == VK_TRUE) { THROW_ERR("Rasterizer depth support not available! (yet...)"); }

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    createInfo.pNext = nullptr; createInfo.flags = 0;   // Optional
    createInfo.depthClampEnable = depthClampEnable;
    createInfo.rasterizerDiscardEnable = VK_FALSE;      // Enabling this will disable any output
    createInfo.polygonMode = polyMode;
    createInfo.cullMode = VK_CULL_MODE_BACK_BIT;        // Set backface culling to support Vulkan cords
    createInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    createInfo.depthBiasEnable = 0.0f;
    createInfo.depthBiasConstantFactor = 0.0f;
    createInfo.depthBiasSlopeFactor = 0.0f;
    createInfo.lineWidth = lineWidth;
}


RasterizerState::~RasterizerState() {
    DEBUG_MSG("Called a RasterizerState struct deconstructor");
}