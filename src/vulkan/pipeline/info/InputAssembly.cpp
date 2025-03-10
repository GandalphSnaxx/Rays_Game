#include "InputAssembly.hpp"
/**
 * @file 
 * @author Ray Richter
 * @brief 
 */

InputAssembly::InputAssembly(const VkPrimitiveTopology topology, const VkBool32 restartEnable) {
    // DEBUG_MSG("Called a InputAssembly struct constructor");
    createInfo_ = {  // Configure the assembly header
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
        nullptr, 0,                                                     // pNext, flags
    };
    createInfo_.topology = topology;
    createInfo_.primitiveRestartEnable = restartEnable;
}