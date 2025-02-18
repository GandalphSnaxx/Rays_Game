#ifndef VULKAN_DEVICE_HPP__
#define VULKAN_DEVICE_HPP__

#include "instance/instance.hpp"
#include "swapchain/swapchain.hpp"
#include <optional>
#include <set>

/// @brief VkDeviceClass is a child class of VkSwapchainOverheadClass, VkCmdQueueOverheadClass, and grandchild of VkOverheadClass. Contains functons relating to device interaction.
///     Instance related requirements: _instance, _enableValidationLayers, _validationLayers.
///     Swapchain related requirements: SwapchainSupportDetails struct, _querySwapchainSupport.
///     Command queue related requirements: QueueFamilyIndicies struct, _findQueueFamilies().
class VkDeviceClass : virtual public VkSwapchainOverheadClass, virtual public VkInstanceOverheadClass {
    public:

    VkDeviceClass();
    ~VkDeviceClass();
    // Delete the default constructor to prevent its use
    // VkDeviceClass::VkDeviceClass() = delete;

    protected:

    /// @brief Queue family indicies
    /// TODO: Move to VkCommandQueueClass
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /// TODO: Move to VkCommandQueueClass
    QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice queriedDevice);

    private:

    /// @brief A list of required extensions
    const std::vector<const char*> _deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
    VkQueue _graphicsQueue;
    VkQueue _presentQueue;

    bool _isDeviceSuitable(VkPhysicalDevice queriedDevice);
    bool _checkDeviceExtensionSupport(VkPhysicalDevice queriedDevice);
    VkResult _pickPhysicalDevice();
    VkResult _createLogicalDevice();
};

#endif