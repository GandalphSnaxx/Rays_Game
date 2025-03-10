#pragma once
/**
 * @file vulkan/devices/properties/DevicePropertiesValidators.hpp
 * @author Ray Richter
 * @brief Class definitions for validating and scoring physical devices for best device selection for Vulkan.
 */
// Includes
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Validator.hpp>

/// @section Device properties validator setup

/// @brief A class that validates VkPhysicalDeviceLimits.
class VkPhysicalDeviceLimitsValidator {
public:
    VReturn_t validate(const VkPhysicalDeviceLimits &qLimits) const {
        VReturn_t score = 0;
        score += maxImageDimension1D_                               ( qLimits.maxImageDimension1D );
        score += maxImageDimension2D_                               ( qLimits.maxImageDimension2D );
        score += maxImageDimension3D_                               ( qLimits.maxImageDimension3D );
        score += maxImageDimensionCube_                             ( qLimits.maxImageDimensionCube );
        score += maxImageArrayLayers_                               ( qLimits.maxImageArrayLayers );
        score += maxTexelBufferElements_                            ( qLimits.maxTexelBufferElements );
        score += maxUniformBufferRange_                             ( qLimits.maxUniformBufferRange );
        score += maxStorageBufferRange_                             ( qLimits.maxStorageBufferRange );
        score += maxPushConstantsSize_                              ( qLimits.maxPushConstantsSize );
        score += maxMemoryAllocationCount_                          ( qLimits.maxMemoryAllocationCount );
        score += maxSamplerAllocationCount_                         ( qLimits.maxSamplerAllocationCount );
        score += bufferImageGranularity_                            ( qLimits.bufferImageGranularity );
        score += sparseAddressSpaceSize_                            ( qLimits.sparseAddressSpaceSize );
        score += maxBoundDescriptorSets_                            ( qLimits.maxBoundDescriptorSets );
        score += maxPerStageDescriptorSamplers_                     ( qLimits.maxPerStageDescriptorSamplers );
        score += maxPerStageDescriptorUniformBuffers_               ( qLimits.maxPerStageDescriptorUniformBuffers );
        score += maxPerStageDescriptorStorageBuffers_               ( qLimits.maxPerStageDescriptorStorageBuffers );
        score += maxPerStageDescriptorSampledImages_                ( qLimits.maxPerStageDescriptorSampledImages );
        score += maxPerStageDescriptorStorageImages_                ( qLimits.maxPerStageDescriptorStorageImages );
        score += maxPerStageDescriptorInputAttachments_             ( qLimits.maxPerStageDescriptorInputAttachments );
        score += maxPerStageResources_                              ( qLimits.maxPerStageResources );
        score += maxDescriptorSetSamplers_                          ( qLimits.maxDescriptorSetSamplers );
        score += maxDescriptorSetUniformBuffers_                    ( qLimits.maxDescriptorSetUniformBuffers );
        score += maxDescriptorSetUniformBuffersDynamic_             ( qLimits.maxDescriptorSetUniformBuffersDynamic );
        score += maxDescriptorSetStorageBuffers_                    ( qLimits.maxDescriptorSetStorageBuffers );
        score += maxDescriptorSetStorageBuffersDynamic_             ( qLimits.maxDescriptorSetStorageBuffersDynamic );
        score += maxDescriptorSetSampledImages_                     ( qLimits.maxDescriptorSetSampledImages );
        score += maxDescriptorSetStorageImages_                     ( qLimits.maxDescriptorSetStorageImages );
        score += maxDescriptorSetInputAttachments_                  ( qLimits.maxDescriptorSetInputAttachments );
        score += maxVertexInputAttributes_                          ( qLimits.maxVertexInputAttributes );
        score += maxVertexInputBindings_                            ( qLimits.maxVertexInputBindings );
        score += maxVertexInputAttributeOffset_                     ( qLimits.maxVertexInputAttributeOffset );
        score += maxVertexInputBindingStride_                       ( qLimits.maxVertexInputBindingStride );
        score += maxVertexOutputComponents_                         ( qLimits.maxVertexOutputComponents );
        score += maxTessellationGenerationLevel_                    ( qLimits.maxTessellationGenerationLevel );
        score += maxTessellationPatchSize_                          ( qLimits.maxTessellationPatchSize );
        score += maxTessellationControlPerVertexInputComponents_    ( qLimits.maxTessellationControlPerVertexInputComponents );
        score += maxTessellationControlPerVertexOutputComponents_   ( qLimits.maxTessellationControlPerVertexOutputComponents );
        score += maxTessellationControlPerPatchOutputComponents_    ( qLimits.maxTessellationControlPerPatchOutputComponents );
        score += maxTessellationControlTotalOutputComponents_       ( qLimits.maxTessellationControlTotalOutputComponents );
        score += maxTessellationEvaluationInputComponents_          ( qLimits.maxTessellationEvaluationInputComponents );
        score += maxTessellationEvaluationOutputComponents_         ( qLimits.maxTessellationEvaluationOutputComponents );
        score += maxGeometryShaderInvocations_                      ( qLimits.maxGeometryShaderInvocations );
        score += maxGeometryInputComponents_                        ( qLimits.maxGeometryInputComponents );
        score += maxGeometryOutputComponents_                       ( qLimits.maxGeometryOutputComponents );
        score += maxGeometryOutputVertices_                         ( qLimits.maxGeometryOutputVertices );
        score += maxGeometryTotalOutputComponents_                  ( qLimits.maxGeometryOutputComponents );
        score += maxFragmentInputComponents_                        ( qLimits.maxFragmentInputComponents );
        score += maxFragmentOutputAttachments_                      ( qLimits.maxFragmentOutputAttachments );
        score += maxFragmentDualSrcAttachments_                     ( qLimits.maxFragmentDualSrcAttachments );
        score += maxFragmentCombinedOutputResources_                ( qLimits.maxFragmentCombinedOutputResources );
        score += maxComputeSharedMemorySize_                        ( qLimits.maxComputeSharedMemorySize );
        score += maxComputeWorkGroupCount_[0]                       ( qLimits.maxComputeWorkGroupCount [0]);
        score += maxComputeWorkGroupCount_[1]                       ( qLimits.maxComputeWorkGroupCount [1]);
        score += maxComputeWorkGroupCount_[2]                       ( qLimits.maxComputeWorkGroupCount [2]);
        score += maxComputeWorkGroupInvocations_                    ( qLimits.maxComputeWorkGroupInvocations );
        score += maxComputeWorkGroupSize_[0]                        ( qLimits.maxComputeWorkGroupSize [0]);
        score += maxComputeWorkGroupSize_[1]                        ( qLimits.maxComputeWorkGroupSize [1]);
        score += maxComputeWorkGroupSize_[2]                        ( qLimits.maxComputeWorkGroupSize [2]);
        score += subPixelPrecisionBits_                             ( qLimits.subPixelPrecisionBits );
        score += subTexelPrecisionBits_                             ( qLimits.subTexelPrecisionBits );
        score += mipmapPrecisionBits_                               ( qLimits.mipmapPrecisionBits );
        score += maxDrawIndexedIndexValue_                          ( qLimits.maxDrawIndexedIndexValue );
        score += maxDrawIndirectCount_                              ( qLimits.maxDrawIndirectCount );
        score += maxSamplerLodBias_                                 ( qLimits.maxSamplerLodBias );
        score += maxSamplerAnisotropy_                              ( qLimits.maxSamplerAnisotropy );
        score += maxViewports_                                      ( qLimits.maxViewports );
        score += maxViewportDimensions_[0]                          ( qLimits.maxViewportDimensions [0]);
        score += maxViewportDimensions_[1]                          ( qLimits.maxViewportDimensions [1]);
        score += viewportBoundsRange_[0]                            ( qLimits.viewportBoundsRange [0]);
        score += viewportBoundsRange_[1]                            ( qLimits.viewportBoundsRange [1]);
        score += viewportSubPixelBits_                              ( qLimits.viewportSubPixelBits );
        score += minMemoryMapAlignment_                             ( qLimits.minMemoryMapAlignment );
        score += minTexelBufferOffsetAlignment_                     ( qLimits.minTexelBufferOffsetAlignment );
        score += minUniformBufferOffsetAlignment_                   ( qLimits.minUniformBufferOffsetAlignment );
        score += minStorageBufferOffsetAlignment_                   ( qLimits.minStorageBufferOffsetAlignment );
        score += minTexelOffset_                                    ( qLimits.minTexelOffset );
        score += maxTexelOffset_                                    ( qLimits.maxTexelOffset );
        score += minTexelGatherOffset_                              ( qLimits.minTexelGatherOffset );
        score += maxTexelGatherOffset_                              ( qLimits.maxTexelGatherOffset );
        score += minInterpolationOffset_                            ( qLimits.minInterpolationOffset );
        score += maxInterpolationOffset_                            ( qLimits.maxInterpolationOffset );
        score += subPixelInterpolationOffsetBits_                   ( qLimits.subPixelInterpolationOffsetBits );
        score += maxFramebufferWidth_                               ( qLimits.maxFramebufferWidth );
        score += maxFramebufferHeight_                              ( qLimits.maxFramebufferHeight );
        score += maxFramebufferLayers_                              ( qLimits.maxFramebufferLayers );
        score += framebufferColorSampleCounts_                      ( qLimits.framebufferColorSampleCounts );
        score += framebufferDepthSampleCounts_                      ( qLimits.framebufferDepthSampleCounts );
        score += framebufferStencilSampleCounts_                    ( qLimits.framebufferStencilSampleCounts );
        score += framebufferNoAttachmentsSampleCounts_              ( qLimits.framebufferNoAttachmentsSampleCounts );
        score += maxColorAttachments_                               ( qLimits.maxColorAttachments );
        score += sampledImageColorSampleCounts_                     ( qLimits.sampledImageColorSampleCounts );
        score += sampledImageIntegerSampleCounts_                   ( qLimits.sampledImageIntegerSampleCounts );
        score += sampledImageDepthSampleCounts_                     ( qLimits.sampledImageDepthSampleCounts );
        score += sampledImageStencilSampleCounts_                   ( qLimits.sampledImageStencilSampleCounts );
        score += storageImageSampleCounts_                          ( qLimits.storageImageSampleCounts );
        score += maxSampleMaskWords_                                ( qLimits.maxSampleMaskWords );
        score += timestampComputeAndGraphics_                       ( qLimits.timestampComputeAndGraphics );
        score += timestampPeriod_                                   ( qLimits.timestampPeriod );
        score += maxClipDistances_                                  ( qLimits.maxClipDistances );
        score += maxCullDistances_                                  ( qLimits.maxCullDistances );
        score += maxCombinedClipAndCullDistances_                   ( qLimits.maxCombinedClipAndCullDistances );
        score += discreteQueuePriorities_                           ( qLimits.discreteQueuePriorities );
        score += pointSizeRange_[0]                                 ( qLimits.pointSizeRange [0]);
        score += pointSizeRange_[1]                                 ( qLimits.pointSizeRange [1]);
        score += lineWidthRange_[0]                                 ( qLimits.lineWidthRange [0]);
        score += lineWidthRange_[1]                                 ( qLimits.lineWidthRange [1]);
        score += pointSizeGranularity_                              ( qLimits.pointSizeGranularity );
        score += lineWidthGranularity_                              ( qLimits.lineWidthGranularity );
        score += strictLines_                                       ( qLimits.strictLines );
        score += standardSampleLocations_                           ( qLimits.standardSampleLocations );
        score += optimalBufferCopyOffsetAlignment_                  ( qLimits.optimalBufferCopyOffsetAlignment );
        score += optimalBufferCopyRowPitchAlignment_                ( qLimits.optimalBufferCopyRowPitchAlignment );
        score += nonCoherentAtomSize_                               ( qLimits.nonCoherentAtomSize );
        return score; }
    VReturn_t operator()(const VkPhysicalDeviceLimits &qLimits) const { return validate(qLimits); }

private:
    VKeyedList_t<uint32_t>              maxImageDimension1D_                            {}; // Any
    VKeyedList_t<uint32_t>              maxImageDimension2D_                            {}; // Any
    VKeyedList_t<uint32_t>              maxImageDimension3D_                            {}; // Any
    VKeyedList_t<uint32_t>              maxImageDimensionCube_                          {}; // Any
    VKeyedList_t<uint32_t>              maxImageArrayLayers_                            {}; // Any
    VKeyedList_t<uint32_t>              maxTexelBufferElements_                         {}; // Any
    VKeyedList_t<uint32_t>              maxUniformBufferRange_                          {}; // Any
    VKeyedList_t<uint32_t>              maxStorageBufferRange_                          {}; // Any
    VKeyedList_t<uint32_t>              maxPushConstantsSize_                           {}; // Any
    VKeyedList_t<uint32_t>              maxMemoryAllocationCount_                       {}; // Any
    VKeyedList_t<uint32_t>              maxSamplerAllocationCount_                      {}; // Any
    VKeyedList_t<VkDeviceSize>          bufferImageGranularity_                         {}; // Any
    VKeyedList_t<VkDeviceSize>          sparseAddressSpaceSize_                         {}; // Any
    VKeyedList_t<uint32_t>              maxBoundDescriptorSets_                         {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorSamplers_                  {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorUniformBuffers_            {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorStorageBuffers_            {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorSampledImages_             {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorStorageImages_             {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageDescriptorInputAttachments_          {}; // Any
    VKeyedList_t<uint32_t>              maxPerStageResources_                           {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetSamplers_                       {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetUniformBuffers_                 {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetUniformBuffersDynamic_          {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetStorageBuffers_                 {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetStorageBuffersDynamic_          {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetSampledImages_                  {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetStorageImages_                  {}; // Any
    VKeyedList_t<uint32_t>              maxDescriptorSetInputAttachments_               {}; // Any
    VKeyedList_t<uint32_t>              maxVertexInputAttributes_                       {}; // Any
    VKeyedList_t<uint32_t>              maxVertexInputBindings_                         {}; // Any
    VKeyedList_t<uint32_t>              maxVertexInputAttributeOffset_                  {}; // Any
    VKeyedList_t<uint32_t>              maxVertexInputBindingStride_                    {}; // Any
    VKeyedList_t<uint32_t>              maxVertexOutputComponents_                      {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationGenerationLevel_                 {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationPatchSize_                       {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationControlPerVertexInputComponents_ {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationControlPerVertexOutputComponents_{}; // Any
    VKeyedList_t<uint32_t>              maxTessellationControlPerPatchOutputComponents_ {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationControlTotalOutputComponents_    {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationEvaluationInputComponents_       {}; // Any
    VKeyedList_t<uint32_t>              maxTessellationEvaluationOutputComponents_      {}; // Any
    VKeyedList_t<uint32_t>              maxGeometryShaderInvocations_                   {}; // Any
    VKeyedList_t<uint32_t>              maxGeometryInputComponents_                     {}; // Any
    VKeyedList_t<uint32_t>              maxGeometryOutputComponents_                    {}; // Any
    VKeyedList_t<uint32_t>              maxGeometryOutputVertices_                      {}; // Any
    VKeyedList_t<uint32_t>              maxGeometryTotalOutputComponents_               {}; // Any
    VKeyedList_t<uint32_t>              maxFragmentInputComponents_                     {}; // Any
    VKeyedList_t<uint32_t>              maxFragmentOutputAttachments_                   {}; // Any
    VKeyedList_t<uint32_t>              maxFragmentDualSrcAttachments_                  {}; // Any
    VKeyedList_t<uint32_t>              maxFragmentCombinedOutputResources_             {}; // Any
    VKeyedList_t<uint32_t>              maxComputeSharedMemorySize_                     {}; // Any
    VKeyedList_t<uint32_t>              maxComputeWorkGroupCount_[3]                    {{}, {}, {}}; // Any, Any, Any
    VKeyedList_t<uint32_t>              maxComputeWorkGroupInvocations_                 {}; // Any
    VKeyedList_t<uint32_t>              maxComputeWorkGroupSize_[3]                     {{}, {}, {}}; // Any, Any, Any
    VKeyedList_t<uint32_t>              subPixelPrecisionBits_                          {}; // Any
    VKeyedList_t<uint32_t>              subTexelPrecisionBits_                          {}; // Any
    VKeyedList_t<uint32_t>              mipmapPrecisionBits_                            {}; // Any
    VKeyedList_t<uint32_t>              maxDrawIndexedIndexValue_                       {}; // Any
    VKeyedList_t<uint32_t>              maxDrawIndirectCount_                           {}; // Any
    VKeyedList_t<float>                 maxSamplerLodBias_                              {}; // Any
    VKeyedList_t<float>                 maxSamplerAnisotropy_                           {}; // Any
    VKeyedList_t<uint32_t>              maxViewports_                                   {}; // Any
    VKeyedList_t<uint32_t>              maxViewportDimensions_[2]                       {{}, {}}; // Any, Any
    VKeyedList_t<float>                 viewportBoundsRange_[2]                         {{}, {}}; // Any, Any
    VKeyedList_t<uint32_t>              viewportSubPixelBits_                           {}; // Any
    VKeyedList_t<size_t>                minMemoryMapAlignment_                          {}; // Any
    VKeyedList_t<VkDeviceSize>          minTexelBufferOffsetAlignment_                  {}; // Any
    VKeyedList_t<VkDeviceSize>          minUniformBufferOffsetAlignment_                {}; // Any
    VKeyedList_t<VkDeviceSize>          minStorageBufferOffsetAlignment_                {}; // Any
    VKeyedList_t<int32_t>               minTexelOffset_                                 {}; // Any
    VKeyedList_t<uint32_t>              maxTexelOffset_                                 {}; // Any
    VKeyedList_t<int32_t>               minTexelGatherOffset_                           {}; // Any
    VKeyedList_t<uint32_t>              maxTexelGatherOffset_                           {}; // Any
    VKeyedList_t<float>                 minInterpolationOffset_                         {}; // Any
    VKeyedList_t<float>                 maxInterpolationOffset_                         {}; // Any
    VKeyedList_t<uint32_t>              subPixelInterpolationOffsetBits_                {}; // Any
    VKeyedList_t<uint32_t>              maxFramebufferWidth_                            {}; // Any
    VKeyedList_t<uint32_t>              maxFramebufferHeight_                           {}; // Any
    VKeyedList_t<uint32_t>              maxFramebufferLayers_                           {}; // Any
    VKeyedList_t<VkSampleCountFlags>    framebufferColorSampleCounts_                   {}; // Any
    VKeyedList_t<VkSampleCountFlags>    framebufferDepthSampleCounts_                   {}; // Any
    VKeyedList_t<VkSampleCountFlags>    framebufferStencilSampleCounts_                 {}; // Any
    VKeyedList_t<VkSampleCountFlags>    framebufferNoAttachmentsSampleCounts_           {}; // Any
    VKeyedList_t<uint32_t>              maxColorAttachments_                            {}; // Any
    VKeyedList_t<VkSampleCountFlags>    sampledImageColorSampleCounts_                  {}; // Any
    VKeyedList_t<VkSampleCountFlags>    sampledImageIntegerSampleCounts_                {}; // Any
    VKeyedList_t<VkSampleCountFlags>    sampledImageDepthSampleCounts_                  {}; // Any
    VKeyedList_t<VkSampleCountFlags>    sampledImageStencilSampleCounts_                {}; // Any
    VKeyedList_t<VkSampleCountFlags>    storageImageSampleCounts_                       {}; // Any
    VKeyedList_t<uint32_t>              maxSampleMaskWords_                             {}; // Any
    VKeyedList_t<VkBool32>              timestampComputeAndGraphics_                    {}; // Any
    VKeyedList_t<float>                 timestampPeriod_                                {}; // Any
    VKeyedList_t<uint32_t>              maxClipDistances_                               {}; // Any
    VKeyedList_t<uint32_t>              maxCullDistances_                               {}; // Any
    VKeyedList_t<uint32_t>              maxCombinedClipAndCullDistances_                {}; // Any
    VKeyedList_t<uint32_t>              discreteQueuePriorities_                        {}; // Any
    VKeyedList_t<float>                 pointSizeRange_[2]                              {{}, {}}; // Any, Any
    VKeyedList_t<float>                 lineWidthRange_[2]                              {{}, {}}; // Any, Any
    VKeyedList_t<float>                 pointSizeGranularity_                           {}; // Any
    VKeyedList_t<float>                 lineWidthGranularity_                           {}; // Any
    VKeyedList_t<VkBool32>              strictLines_                                    {}; // Any
    VKeyedList_t<VkBool32>              standardSampleLocations_                        {}; // Any
    VKeyedList_t<VkDeviceSize>          optimalBufferCopyOffsetAlignment_               {}; // Any
    VKeyedList_t<VkDeviceSize>          optimalBufferCopyRowPitchAlignment_             {}; // Any
    VKeyedList_t<VkDeviceSize>          nonCoherentAtomSize_                            {}; // Any
};
    
/// @brief A class that validates VkPhysicalDeviceSparseProperties.
class VkPhysicalDeviceSparsePropertiesValidator {
public:
    VReturn_t validate(const VkPhysicalDeviceSparseProperties &qLimits) const {
        VReturn_t score = 0;
        score += residencyStandard2DBlockShape_             (qLimits.residencyStandard2DBlockShape);
        score += residencyStandard2DMultisampleBlockShape_  (qLimits.residencyStandard2DMultisampleBlockShape);
        score += residencyStandard3DBlockShape_             (qLimits.residencyStandard3DBlockShape);
        score += residencyAlignedMipSize_                   (qLimits.residencyAlignedMipSize);
        score += residencyNonResidentStrict_                (qLimits.residencyNonResidentStrict);
        return score; }
    VReturn_t operator()(const VkPhysicalDeviceSparseProperties &qLimits) const { return validate(qLimits); }
private:
    VKeyedList_t<VkBool32> residencyStandard2DBlockShape_{{
        {0, VK_FALSE}, {1, VK_TRUE}}}; // True = 1 point, false != fail
    VKeyedList_t<VkBool32> residencyStandard2DMultisampleBlockShape_{{
        {0, VK_FALSE}, {1, VK_TRUE}}}; // True = 1 point, false != fail
    VKeyedList_t<VkBool32> residencyStandard3DBlockShape_{{
        {0, VK_FALSE}, {1, VK_TRUE}}}; // True = 1 point, false != fail
    VKeyedList_t<VkBool32> residencyAlignedMipSize_{{
        {0, VK_FALSE}, {1, VK_TRUE}}}; // True = 1 point, false != fail
    VKeyedList_t<VkBool32> residencyNonResidentStrict_{{
        {0, VK_FALSE}, {1, VK_TRUE}}}; // True = 1 point, false != fail
};

/// @brief A class that validates VkPhysicalDeviceProperties.
class VkPhysicalDevicePropertiesValidator {
public:
    VReturn_t validate(const VkPhysicalDeviceProperties &qProperties) const {
        VReturn_t score = 0;
        score += apiVersion_        (qProperties.apiVersion         );
        score += driverVersion_     (qProperties.driverVersion      );
        score += vendorID_          (qProperties.vendorID           );
        score += deviceID_          (qProperties.deviceID           );
        score += deviceType_        (qProperties.deviceType         );
        // score += deviceName_        (qProperties.deviceName         );
        // score += pipelineCacheUUID_ (qProperties.pipelineCacheUUID  );
        score += limits_            (qProperties.limits             );
        score += sparseProperties_  (qProperties.sparseProperties   );
        return score; }
    VReturn_t operator()(const VkPhysicalDeviceProperties &qProperties) const { return validate(qProperties); }
private:
    VKeyedList_t<uint32_t>                                  apiVersion_         {}; // Any
    VKeyedList_t<uint32_t>                                  driverVersion_      {}; // Any
    VKeyedList_t<uint32_t>                                  vendorID_           {}; // Any
    VKeyedList_t<uint32_t>                                  deviceID_           {}; // Any
    VKeyedList_t<VkPhysicalDeviceType>                      deviceType_         {{  // Keyed List
        {VKey_t::BLACKLIST, VK_PHYSICAL_DEVICE_TYPE_OTHER           },
        {10               , VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  },
        {100              , VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    },
        {20               , VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU     },
        {1                , VK_PHYSICAL_DEVICE_TYPE_CPU             },
        {VKey_t::BLACKLIST, VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM        }           }}; // End Keyed List
    // VKeyedList_t<char[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE]>    deviceName_         {}; // Any
    // VKeyedList_t<uint8_t[VK_UUID_SIZE]>                     pipelineCacheUUID_  {}; // Any
    VkPhysicalDeviceLimitsValidator                         limits_             {}; // Limits set in sub class
    VkPhysicalDeviceSparsePropertiesValidator               sparseProperties_   {}; // Limits set in sub class
};

/// @brief A class that validates VkPhysicalDeviceFeatures.
class VkPhysicalDeviceFeaturesValidator {
public:
    VReturn_t validate(const VkPhysicalDeviceFeatures &qFeatures) const {
        VReturn_t score = 0;
        score += validateBool_ (qFeatures.robustBufferAccess);
        score += validateBool_ (qFeatures.fullDrawIndexUint32);
        score += validateBool_ (qFeatures.imageCubeArray);
        score += validateBool_ (qFeatures.independentBlend);
        score += validateBool_ (qFeatures.geometryShader);
        score += validateBool_ (qFeatures.tessellationShader);
        score += validateBool_ (qFeatures.sampleRateShading);
        score += validateBool_ (qFeatures.dualSrcBlend);
        score += validateBool_ (qFeatures.logicOp);
        score += validateBool_ (qFeatures.multiDrawIndirect);
        score += validateBool_ (qFeatures.drawIndirectFirstInstance);
        score += validateBool_ (qFeatures.depthClamp);
        score += validateBool_ (qFeatures.depthBiasClamp);
        score += validateBool_ (qFeatures.fillModeNonSolid);
        score += validateBool_ (qFeatures.depthBounds);
        score += validateBool_ (qFeatures.wideLines);
        score += validateBool_ (qFeatures.largePoints);
        score += validateBool_ (qFeatures.alphaToOne);
        score += validateBool_ (qFeatures.multiViewport);
        score += validateBool_ (qFeatures.samplerAnisotropy);
        score += validateBool_ (qFeatures.textureCompressionETC2);
        score += validateBool_ (qFeatures.textureCompressionASTC_LDR);
        score += validateBool_ (qFeatures.textureCompressionBC);
        score += validateBool_ (qFeatures.occlusionQueryPrecise);
        score += validateBool_ (qFeatures.pipelineStatisticsQuery);
        score += validateBool_ (qFeatures.vertexPipelineStoresAndAtomics);
        score += validateBool_ (qFeatures.fragmentStoresAndAtomics);
        score += validateBool_ (qFeatures.shaderTessellationAndGeometryPointSize);
        score += validateBool_ (qFeatures.shaderImageGatherExtended);
        score += validateBool_ (qFeatures.shaderStorageImageExtendedFormats);
        score += validateBool_ (qFeatures.shaderStorageImageMultisample);
        score += validateBool_ (qFeatures.shaderStorageImageReadWithoutFormat);
        score += validateBool_ (qFeatures.shaderStorageImageWriteWithoutFormat);
        score += validateBool_ (qFeatures.shaderUniformBufferArrayDynamicIndexing);
        score += validateBool_ (qFeatures.shaderSampledImageArrayDynamicIndexing);
        score += validateBool_ (qFeatures.shaderStorageBufferArrayDynamicIndexing);
        score += validateBool_ (qFeatures.shaderStorageImageArrayDynamicIndexing);
        score += validateBool_ (qFeatures.shaderClipDistance);
        score += validateBool_ (qFeatures.shaderCullDistance);
        score += validateBool_ (qFeatures.shaderFloat64);
        score += validateBool_ (qFeatures.shaderInt64);
        score += validateBool_ (qFeatures.shaderInt16);
        score += validateBool_ (qFeatures.shaderResourceResidency);
        score += validateBool_ (qFeatures.shaderResourceMinLod);
        score += validateBool_ (qFeatures.sparseBinding);
        score += validateBool_ (qFeatures.sparseResidencyBuffer);
        score += validateBool_ (qFeatures.sparseResidencyImage2D);
        score += validateBool_ (qFeatures.sparseResidencyImage3D);
        score += validateBool_ (qFeatures.sparseResidency2Samples);
        score += validateBool_ (qFeatures.sparseResidency4Samples);
        score += validateBool_ (qFeatures.sparseResidency8Samples);
        score += validateBool_ (qFeatures.sparseResidency16Samples);
        score += validateBool_ (qFeatures.sparseResidencyAliased);
        score += validateBool_ (qFeatures.variableMultisampleRate);
        score += validateBool_ (qFeatures.inheritedQueries);
        return score; }
    VReturn_t operator()(const VkPhysicalDeviceFeatures &qFeatures) const { return validate(qFeatures); }
private:
    VKeyedList_t<VkBool32> validateBool_ {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // For more specific scoring, uncomment and edit the keyed list definitions below.
    // VKeyedList_t<VkBool32> robustBufferAccess_                      {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> fullDrawIndexUint32_                     {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> imageCubeArray_                          {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> independentBlend_                        {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> geometryShader_                          {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> tessellationShader_                      {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sampleRateShading_                       {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> dualSrcBlend_                            {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> logicOp_                                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> multiDrawIndirect_                       {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> drawIndirectFirstInstance_               {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> depthClamp_                              {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> depthBiasClamp_                          {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> fillModeNonSolid_                        {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> depthBounds_                             {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> wideLines_                               {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> largePoints_                             {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> alphaToOne_                              {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> multiViewport_                           {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> samplerAnisotropy_                       {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> textureCompressionETC2_                  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> textureCompressionASTC_LDR_              {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> textureCompressionBC_                    {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> occlusionQueryPrecise_                   {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> pipelineStatisticsQuery_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> vertexPipelineStoresAndAtomics_          {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> fragmentStoresAndAtomics_                {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderTessellationAndGeometryPointSize_  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderImageGatherExtended_               {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageImageExtendedFormats_       {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageImageMultisample_           {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageImageReadWithoutFormat_     {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageImageWriteWithoutFormat_    {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderUniformBufferArrayDynamicIndexing_ {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderSampledImageArrayDynamicIndexing_  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageBufferArrayDynamicIndexing_ {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderStorageImageArrayDynamicIndexing_  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderClipDistance_                      {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderCullDistance_                      {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderFloat64_                           {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderInt64_                             {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderInt16_                             {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderResourceResidency_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> shaderResourceMinLod_                    {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseBinding_                           {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidencyBuffer_                   {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidencyImage2D_                  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidencyImage3D_                  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidency2Samples_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidency4Samples_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidency8Samples_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidency16Samples_                {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> sparseResidencyAliased_                  {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> variableMultisampleRate_                 {{{0, VK_FALSE}, {1, VK_TRUE}}};
    // VKeyedList_t<VkBool32> inheritedQueries_                        {{{0, VK_FALSE}, {1, VK_TRUE}}};
};