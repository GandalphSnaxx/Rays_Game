#include "overhead.hpp"

// Input assembly states describing what kind of geometry will be drawn and should primitive restart be enabled
// Points from vertices
const VkPipelineInputAssemblyStateCreateInfo INPUT_ASSEMBLY_POINT_LIST = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
    nullptr, 0,                                                     // pNext, flags
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST,                               // topology
    VK_FALSE                                                        // primitiveRestartEnable
};
// Line from every 2 vertices without reuse
const VkPipelineInputAssemblyStateCreateInfo INPUT_ASSEMBLY_LINE_LIST = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
    nullptr, 0,                                                     // pNext, flags
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST,                                // topology
    VK_FALSE                                                        // primitiveRestartEnable
};
// The end vertex of every line is used as start vertex for the next line
const VkPipelineInputAssemblyStateCreateInfo INPUT_ASSEMBLY_LINE_STRIP = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
    nullptr, 0,                                                     // pNext, flags
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,                               // topology
    VK_FALSE                                                        // primitiveRestartEnable
};
// Triangle from every 3 vertices without reuse
const VkPipelineInputAssemblyStateCreateInfo INPUT_ASSEMBLY_TRIANGLE_LIST = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
    nullptr, 0,                                                     // pNext, flags
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,                            // topology
    VK_FALSE                                                        // primitiveRestartEnable
};
// The second and third vertex of every triangle are used as first two vertices of the next triangle
const VkPipelineInputAssemblyStateCreateInfo INPUT_ASSEMBLY_TRIANGLE_STRIP = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType
    nullptr, 0,                                                     // pNext, flags
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,                           // topology
    VK_FALSE                                                        // primitiveRestartEnable
};

// Rasterizer states. The rasterizer takes the geometry that is shaped by the vertices from the vertex shader and turns it into fragments 
//  to be colored by the fragment shader. It also performs depth testing, face culling and the scissor test, and it can be configured to 
//  output fragments that fill entire polygons or just the edges (wireframe rendering). All this is configured using the 
//  VkPipelineRasterizationStateCreateInfo structure.
// cullMode and frontFace are set for Vulkan cords

// Fill polygons
const VkPipelineRasterizationStateCreateInfo RASTERIZER_FILL = {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // sType
    nullptr, 0,                                                 // pNext, flags
    VK_FALSE,                                                   // depthClampEnable
    VK_FALSE,                                                   // rasterizerDiscardEnable
    VK_POLYGON_MODE_FILL,                                       // polygonMode
    VK_CULL_MODE_BACK_BIT,                                      // cullMode
    VK_FRONT_FACE_COUNTER_CLOCKWISE,                            // frontFace
    VK_FALSE,                                                   // depthBiasEnable
    0.0f,                                                       // depthBiasConstantFactor
    0.0f,                                                       // depthBiasClamp
    0.0f,                                                       // depthBiasSlopeFactor
    1.0f                                                        // lineWidth
};
// Display vertex lines (wireframe)
const VkPipelineRasterizationStateCreateInfo RASTERIZER_LINE = {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // sType
    nullptr, 0,                                                 // pNext, flags
    VK_FALSE,                                                   // depthClampEnable
    VK_FALSE,                                                   // rasterizerDiscardEnable
    VK_POLYGON_MODE_LINE,                                       // polygonMode
    VK_CULL_MODE_BACK_BIT,                                      // cullMode
    VK_FRONT_FACE_COUNTER_CLOCKWISE,                            // frontFace
    VK_FALSE,                                                   // depthBiasEnable
    0.0f,                                                       // depthBiasConstantFactor
    0.0f,                                                       // depthBiasClamp
    0.0f,                                                       // depthBiasSlopeFactor
    1.0f                                                        // lineWidth
};
// Display vertex points
const VkPipelineRasterizationStateCreateInfo RASTERIZER_POINT = {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // sType
    nullptr, 0,                                                 // pNext, flags
    VK_FALSE,                                                   // depthClampEnable
    VK_FALSE,                                                   // rasterizerDiscardEnable
    VK_POLYGON_MODE_POINT,                                      // polygonMode
    VK_CULL_MODE_BACK_BIT,                                      // cullMode
    VK_FRONT_FACE_COUNTER_CLOCKWISE,                            // frontFace
    VK_FALSE,                                                   // depthBiasEnable
    0.0f,                                                       // depthBiasConstantFactor
    0.0f,                                                       // depthBiasClamp
    0.0f,                                                       // depthBiasSlopeFactor
    1.0f                                                        // lineWidth
};

// Multisampling configurations. Can be used for anti-aliasing if supported by gpu

// No multisampling
const VkPipelineMultisampleStateCreateInfo MULTISAMPLING_DISABLE = {
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,   // sType
    nullptr, 0,                                                 // pNext, flags
    VK_SAMPLE_COUNT_1_BIT,                                      // rasterizationSamples
    VK_FALSE,                                                   // sampleShadingEnable
    1.0f,                                                       // minSampleShading
    nullptr,                                                    // pSampleMask
    VK_FALSE,                                                   // alphaToCoverageEnable
    VK_FALSE                                                    // alphaToOneEnable
};

/// TODO: Depth and stencil buffer config

// Color blend attachment config section. Color blending modes attached to each framebuffer
/* Color blending pseudo code:
if (blendEnable) {
    finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
    finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
} else {
    finalColor = newColor;
}

finalColor = finalColor & colorWriteMask;

The most common way to use color blending is to implement alpha blending, where we want the new color to be blended with the old color 
based on its opacity. The finalColor should then be computed as follows:
finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
finalColor.a = newAlpha.a;
*/

// Disable any output
const VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_NULL = {
    VK_FALSE,                   // blendEnable
    VK_BLEND_FACTOR_ZERO,       // srcColorBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstColorBlendFactor
    VK_BLEND_OP_ADD,            // colorBlendOp
    VK_BLEND_FACTOR_ZERO,       // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,            // alphaBlendOp
    0                           // colorWriteMask bitfield
};
// Disable color blending attachments but keep the output
const VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_NO_BLEND = {
    VK_FALSE,                   // blendEnable
    VK_BLEND_FACTOR_ONE,        // srcColorBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstColorBlendFactor
    VK_BLEND_OP_ADD,            // colorBlendOp
    VK_BLEND_FACTOR_ONE,        // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,       // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,            // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |  // colorWriteMask bitfield
    VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | 
    VK_COLOR_COMPONENT_A_BIT
};
// Enable standard alpha blending
const VkPipelineColorBlendAttachmentState COLOR_BLEND_ATTACHMENT_ALPHA_BLEND = {
    VK_TRUE,                                // blendEnable
    VK_BLEND_FACTOR_SRC_ALPHA,              // srcColorBlendFactor
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,    // dstColorBlendFactor
    VK_BLEND_OP_ADD,                        // colorBlendOp
    VK_BLEND_FACTOR_ONE,                    // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                   // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                        // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |              // colorWriteMask bitfield
    VK_COLOR_COMPONENT_G_BIT | 
    VK_COLOR_COMPONENT_B_BIT | 
    VK_COLOR_COMPONENT_A_BIT
};