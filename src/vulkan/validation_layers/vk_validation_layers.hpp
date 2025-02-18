#ifndef VK_VALIDATION_LAYERS_HPP__
#define VK_VALIDATION_LAYERS_HPP__

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <set>

#include "vulkan/debug_utils.hpp"
#include "raysDebugHelper.hpp"

class VkValidationLayersClass {
    public:
    
    VkValidationLayersClass();
    ~VkValidationLayersClass();

    // Public member functions
    bool checkValidationLayerSupport();
    std::vector<const char*> getValidationLayers();
    std::vector<const char*> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult init(VkInstance *instance);

    /// @brief Get the size of the validation layer list
    /// @return The size of the validation layer list
    inline size_t size() { return validationLayers.size(); }

    /// @brief Gets the data from the validation layer list
    /// @return Data
    inline const char *const *data() { return validationLayers.data(); }

    /// @brief A check for validation layers being enabled
    /// @return True if enabled
    inline bool enabled() { return enableValidationLayers; }

    private:

    // Private member variables
    VkInstance *validationInstance; // A pointer to the instance we want to add a validation layer to
    VkDebugUtilsMessengerEXT debugMessenger;

    // A list of validation layers
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // If DEBUG is defined, enable validation layers
#ifdef DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

    // Private member functions
};

#endif