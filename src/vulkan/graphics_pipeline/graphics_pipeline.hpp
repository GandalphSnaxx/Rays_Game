#ifndef VULKAN_GRAPHICS_PIPELINE_HPP__
#define VULKAN_GRAPHICS_PIPELINE_HPP__

#include "pipeline_config/structs.hpp"
#include "swapchain/swapchain.hpp"
#include "memory/memory_mgmt.hpp"
#include <array>

/// @brief VkPipelineOverheadClass holds functions and variables for handling a graphics pipeline before initalization. VkPipelineClass is a child of VkUBOOverheadClass and VkSwapchainOverheadClass.
///     Requires access to: _descriptorSetLayout, _renderPass
class VkPipelineOverheadClass : virtual public VkUBOOverheadClass, virtual public VkSwapchainOverheadClass {
    public:
    VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files);
    ~VkPipelineOverheadClass();

    protected:
    VkPipelineLayout _pipelineLayout;

    VkGraphicsPipelineCreateInfo *_getCreateInfo() { return &_pipelineInfo; }

    private:
    ShaderStage         _shaderStages;
    VertexInput         _vertexInputs;
    InputAssembly       _inputAssembly;
    ViewportState       _viewportState;
    RasterizerState     _rasterizer;
    MultisamplingState  _multisampling;
    ColorBlendState     _colorBlending;

    // PipelineInfo _pipelineInfo;

    VkPipelineInputAssemblyStateCreateInfo _inputAssembly{};
    void _initInputAssembly(const VkPipelineInputAssemblyStateCreateInfo inputAssemblyType);

    VkPipelineViewportStateCreateInfo _viewportState{};
    void _initViewportState();
    VkPipelineRasterizationStateCreateInfo _rasterizer{};
    void _initRasterizer(const VkPipelineRasterizationStateCreateInfo rasterizerType);
    VkPipelineMultisampleStateCreateInfo _multisampling{};
    void _initMultisampling(const VkPipelineMultisampleStateCreateInfo multisamplingConfig);
    VkPipelineColorBlendAttachmentState _colorBlendAttachment{};
    void _initColorBlendAttachment(const VkPipelineColorBlendAttachmentState colorBlendAttachmentConfig);
    VkPipelineColorBlendStateCreateInfo _colorBlending{};
    void _initColorBlend(const bool enableLogicOp, const VkLogicOp operation);
    VkGraphicsPipelineCreateInfo _pipelineInfo{};
    void _initPipelineInfo();
};

/// @brief VkPipelineClass holds functions and variables related to the graphics pipeline. VkPipelineClass is a child of VkUBOOverheadClass and VkSwapchainOverheadClass.
///     Requires access to: _descriptorSetLayout, _renderPass
class VkPipelineClass : virtual public VkPipelineOverheadClass {
    public:
    VkPipelineClass();
    ~VkPipelineClass();

    protected:
    
    private:
    VkPipeline _gfxPipeline;

    VkResult _initPipeline();
};

#endif