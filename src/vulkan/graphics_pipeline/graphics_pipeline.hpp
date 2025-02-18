#ifndef VULKAN_GRAPHICS_PIPELINE_HPP__
#define VULKAN_GRAPHICS_PIPELINE_HPP__

#include "swapchain/swapchain.hpp"
#include "memory/memory_mgmt.hpp"
#include <array>
#include <fstream>
#include <vector>

/// @brief VkPipelineOverheadClass holds functions and variables for handling a graphics pipeline before initalization. VkPipelineClass is a child of VkUBOOverheadClass and VkSwapchainOverheadClass.
///     Requires access to: _descriptorSetLayout, _renderPass
class VkPipelineOverheadClass : virtual public VkUBOOverheadClass, virtual public VkSwapchainOverheadClass {
    public:
    VkPipelineOverheadClass(const std::vector<ShaderFile> &shader_files);
    ~VkPipelineOverheadClass();

    /// @brief A struct for storing vertex information and member functions
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        // A member function for getting the binding description
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        // A member function for getting attribute descriptions
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

    protected:
    VkPipelineLayout _pipelineLayout;

    VkGraphicsPipelineCreateInfo *_getCreateInfo() { return &_pipelineInfo; }

    private:
    static std::vector<char> _readFile(const std::string& filename);
    VkShaderModule _createShaderModule(const std::vector<char>& code);
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    void _initShaderStages(const std::vector<ShaderFile> &shader_files);
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo{};
    void _initVertexInput();
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly{};
    void _initInputAssembly();
    VkPipelineViewportStateCreateInfo _viewportState{};
    void _initViewportState();
    VkPipelineRasterizationStateCreateInfo _rasterizer{};
    void _initRasterizer();
    VkPipelineMultisampleStateCreateInfo _multisampling{};
    void _initMultisampling();
    VkPipelineColorBlendAttachmentState _colorBlendAttachment{};
    void _initColorBlendAttachment();
    VkPipelineColorBlendStateCreateInfo _colorBlending{};
    void _initColorBlend();
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