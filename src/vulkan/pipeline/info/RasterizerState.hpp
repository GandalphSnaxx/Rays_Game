#pragma once
/**
 * @file vulkan/pipeline/info/RasterizerState.hpp
 * @author Ray Richter
 * @brief Rasterizer state create info for pipeline creation.
 */
#include <vulkan/vulkan.h>
#include "raysDebugHelper.hpp"

/// @brief A struct for storing rasterizer state info for pipeline creation
struct RasterizerState {
public:
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
    RasterizerState(
        const VkPolygonMode polyMode = VK_POLYGON_MODE_FILL, 
        const float lineWidth = 1.0f, 
        const VkBool32 depthClampEnable = VK_FALSE);
    ~RasterizerState();

    inline const VkPipelineRasterizationStateCreateInfo* data() const noexcept { return &createInfo_; }

private:
    VkPipelineRasterizationStateCreateInfo createInfo_{};

    /// TODO: Create these functions
    bool wideLinesSupported_();
    bool depthBufferSupported_();
};