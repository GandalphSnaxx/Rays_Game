#include "overhead.hpp"
#include "constants.hpp"
#include <array>
#include <fstream>

/// @brief A struct holding VkPipelineDynamicStateCreateInfo related info for pipeline creation
struct DynamicStateStage {
    VkPipelineDynamicStateCreateInfo createInfo{};

    DynamicStateStage(const DynamicStateList *dynamicStates);
    ~DynamicStateStage();
    // Delete default constructor so it is not accidentally used
    DynamicStateStage::DynamicStateStage() = delete;
};

/// @brief A struct holding a list of VkPipelineShaderStageCreateInfo and functions for initalizing them.
struct ShaderStage {
    std::vector<VkPipelineShaderStageCreateInfo> createInfo;
    
    ShaderStage(const std::vector<ShaderFile> &shaderFiles, VkDevice device);
    /// TODO: Destroy shader modules after pipeline creation
    ~ShaderStage();
    // Delete default constructor so it is not accidentally used
    ShaderStage::ShaderStage() = delete;

    private:
    const VkDevice *_device;
    std::vector<VkShaderModule> _shaderModules;
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

    ViewportState(const DynamicStateList *dynamicStates);
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

/// @brief A struct holding VkPipelineColorBlendStateCreateInfo related info for pipeline creation
struct ColorBlendState {
    VkPipelineColorBlendStateCreateInfo createInfo{};

    ColorBlendState(const std::vector<VkPipelineColorBlendAttachmentState> *blendModes, const VkLogicOp logicOp);
    ~ColorBlendState();
    // Delete default constructor so it is not accidentally used
    ColorBlendState::ColorBlendState() = delete;

    private:
    std::array<float, 4> _blendConstants = {0.0f, 0.0f, 0.0f, 0.0f};
    // std::vector<VkPipelineColorBlendAttachmentState> _attachmentStates{};
};


struct PipelineLayout {
    VkPipelineLayoutCreateInfo createInfo{};

    PipelineLayout(const std::vector<VkDescriptorSetLayout> *descriptorSetLayouts);
    ~PipelineLayout();
    // Delete default constructor so it is not accidentally used
    PipelineLayout::PipelineLayout() = delete;
};