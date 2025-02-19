#include "overhead.hpp"
#include "constants.hpp"
#include <fstream>

/// @brief A struct holding a list of VkPipelineShaderStageCreateInfo and functions for initalizing them.
struct ShaderStage {
    std::vector<VkPipelineShaderStageCreateInfo> createInfo;
    
    ShaderStage(const std::vector<ShaderFile> &shaderFiles, VkDevice device);
    ~ShaderStage();
    // Delete default constructor so it is not accidentally used
    ShaderStage::ShaderStage() = delete;

    private:
    VkResult _createShaderModule(const ShaderFile *shaderFile, VkDevice device, VkShaderModule *shaderModule);
};

/// @brief A struct for holding VkPipelineVertexInputStateCreateInfo and variables related to it.
struct VertexInput {
    std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = Vertex::getBindingDescription();
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo createInfo{};
    
    VertexInput();
    ~VertexInput();
};

/// @brief A struct for holding VkPipelineInputAssemblyStateCreateInfo
struct InputAssembly {
    VkPipelineInputAssemblyStateCreateInfo createInfo{};
    
    InputAssembly(const VkPrimitiveTopology topology, const VkBool32 restartEnable);
    ~InputAssembly();
    // Delete the default constructor so it is not used
    InputAssembly::InputAssembly() = delete;
};

/// @brief A struct for storing ViewportState related info for pipeline creation
struct ViewportState {
    VkPipelineViewportStateCreateInfo createInfo{};

    ViewportState(const DynamicStateList dynamicStates);
    ~ViewportState();
    // Delete default constructor so it is not accidentally used
    ViewportState::ViewportState() = delete;
};

/// @brief A struct for storing rasterizer state info for pipeline creation
struct RasterizerState {
    VkPipelineRasterizationStateCreateInfo createInfo{};

    RasterizerState(const VkPolygonMode polyMode, const float lineWidth, const VkBool32 depthClampEnable);
    RasterizerState(const VkPolygonMode polyMode, const float lineWidth);
    RasterizerState(const VkPolygonMode polyMode);
    ~RasterizerState();
    // Delete default constructor so it is not accidentally used
    RasterizerState::RasterizerState() = delete;

    private:
    /// TODO: Create these functions
    bool _wideLinesSupported();
    bool _depthBufferSupported();
};

/// @brief A struct for initalizing multisampling info for pipeline creation
struct MultisamplingState {
    VkPipelineMultisampleStateCreateInfo createInfo{};

    MultisamplingState(const VkSampleCountFlagBits rasterizationSamples);
    ~MultisamplingState();
    // Delete default constructor so it is not accidentally used
    MultisamplingState::MultisamplingState() = delete;

    private:
    /// TODO: Check for anti-aliasing support
    bool _sampleRateShadingEnabled();
    bool _alphaToOneEnabled();
    bool _mixedSamplesEnabled();
    bool _aaSupported();
};

struct ColorBlendState {
    VkPipelineColorBlendStateCreateInfo createInfo{};

    ColorBlendState();
    ~ColorBlendState();
    // Delete default constructor so it is not accidentally used
    ColorBlendState::ColorBlendState() = delete;

    private:
    VkPipelineColorBlendAttachmentState _attachmentState{};
};