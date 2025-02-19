#ifndef VULKAN_GRAPHICS_PIPELINE_HPP__
#define VULKAN_GRAPHICS_PIPELINE_HPP__

#include "pipeline_config/structs.hpp"
#include "swapchain/swapchain.hpp"
#include "memory/memory_mgmt.hpp"
#include <array>

/// @brief VkPipelineOverheadClass holds functions and variables for handling a graphics pipeline before initalization. VkPipelineClass is a child of VkUBOOverheadClass and VkSwapchainOverheadClass.
///     Requires access to: _descriptorSetLayout, _renderPass, _device
class VkPipelineOverheadClass : virtual public VkUBOOverheadClass, virtual public VkSwapchainOverheadClass {
    public:
    VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files);
    ~VkPipelineOverheadClass();

    protected:
    VkPipelineLayout _pipelineLayout;
    VkGraphicsPipelineCreateInfo _pipelineInfo;

    void _cleanupShaderStages() { _shaderStages.~ShaderStage(); }

    private:
    ShaderStage         _shaderStages;
    VertexInput         _vertexInputs;
    InputAssembly       _inputAssembly;
    ViewportState       _viewportState;
    RasterizerState     _rasterizer;
    MultisamplingState  _multisampling;
    ColorBlendState     _colorBlending;
    DynamicStateStage   _dynamicStates;
    PipelineLayout      _pipelineLayoutInfo;
    // PipelineInfo _pipelineInfo;
};

/// @brief VkPipelineClass holds functions and variables related to the graphics pipeline. VkPipelineClass is a child of VkUBOOverheadClass and VkSwapchainOverheadClass.
///     Requires access to: _descriptorSetLayout, _renderPass
class VkPipelineClass : virtual public VkPipelineOverheadClass {
    public:
    VkPipelineClass(const std::vector<ShaderFile> &shader_files);
    ~VkPipelineClass();

    protected:
    
    private:
    VkPipeline _gfxPipeline;
};

#endif