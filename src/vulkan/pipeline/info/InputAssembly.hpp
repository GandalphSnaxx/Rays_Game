#pragma once
/**
 * @file vulkan/pipeline/info/InputAssembly.hpp
 * @author Ray Richter
 * @brief Input assembly struct for pipeline creation
 */
#include <vulkan/vulkan.h>

/// @brief A struct for holding VkPipelineInputAssemblyStateCreateInfo
struct InputAssembly {
public:    
    /// @brief Initalizes an InputAssembly struct and a VkPipelineInputAssemblyStateCreateInfo.
    /// @param topology Describes how to interperate vertex data
    ///
    ///  - VK_PRIMITIVE_TOPOLOGY_POINT_LIST: points from vertices
    ///
    ///  - VK_PRIMITIVE_TOPOLOGY_LINE_LIST: line from every 2 vertices without reuse
    ///
    ///  - VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: the end vertex of every line is used as start vertex for the next line
    ///
    ///  - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: triangle from every 3 vertices without reuse
    ///
    ///  - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: the second and third vertex of every triangle are used as first two vertices of the next triangle
    ///
    ///  .
    /// @param restartEnable If you set the primitiveRestartEnable member to `VK_TRUE`, then it's possible to break up lines and 
    ///     triangles in the _STRIP topology modes by using a special index of `0xFFFF` or `0xFFFFFFFF`. Default = `VK_FALSE`
    InputAssembly(const VkPrimitiveTopology topology, const VkBool32 restartEnable = VK_FALSE);

    inline const VkPipelineInputAssemblyStateCreateInfo* data() const noexcept { return &createInfo_; }

private:
    VkPipelineInputAssemblyStateCreateInfo createInfo_{};
};