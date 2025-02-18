#ifndef VK_GRAPHICS_PIPELINE_HPP__
#define VK_GRAPHICS_PIPELINE_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <array>
#include <fstream>

// #include "vulkan/descriptors/vk_descriptors.hpp"
// #include "vulkan/devices/vk_devices.hpp"
// #include "vulkan/instance/vk_instance.hpp"
#include "raysDebugHelper.hpp"

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

class VkPipelineClass {
    public:

    // Constructor and deconstructor
    VkPipelineClass();
    ~VkPipelineClass();

    VkResult init(VkDescriptorClass *pDescriptors);

    private:

    const std::string FRAG_SHADER_FILE = "src/shaders/fragment/frag.spv";
    const std::string VERT_SHADER_FILE = "src/shaders/vertex/vert.spv";

    
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkInstanceClass *instance;
    VkDescriptorClass *descriptors;
    VkDevicesClass *device;

    static std::vector<char> readFile(const std::string& filename);
};

#endif