#pragma once
/**
 * @file vulkan/pipeline/info/ShaderStage.hpp
 * @author Ray Richter
 * @brief Shader module handler for pipeline creation.
 */
#include <vulkan/vulkan.h>
#include <vector>
#include "ShaderFile.hpp"

/// @brief A struct holding a list of VkPipelineShaderStageCreateInfo and functions for initalizing them.
struct ShaderStage {
    
    ShaderStage(const std::vector<ShaderFile> &shaderFiles, const VkDevice &device);
    /// TODO: Destroy shader modules after pipeline creation
    ~ShaderStage();
    // Delete default constructor so it is not accidentally used
    ShaderStage::ShaderStage() = delete;

    inline       uint32_t                         size() const noexcept { return createInfo_.size(); }
    inline const VkPipelineShaderStageCreateInfo* data() const noexcept { return createInfo_.data(); }

private:
    VkDevice device_;
    std::vector<VkShaderModule> shaderModules_;
    std::vector<VkPipelineShaderStageCreateInfo> createInfo_{};

    VkResult createShaderModule_(const ShaderFile &shaderFile, VkShaderModule *shaderModule);
};