#include <vulkan/vulkan.h>
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#include <windows.h>
#elif // Different OS
#endif
#include <iostream>

#define ArraySize(arr) sizeof(arr) / sizeof(arr[0])

#define VK_CHECK(result)                                        \
    if (result != VK_SUCCESS) {                                 \
        std::cout << "Vulkan error: " << result << std::endl;   \
        __debugbreak();                                         \
        return false;                                           \
    }

// Debug callback
static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cout << "Validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

struct VkContext {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    VkPhysicalDevice gpu;
    VkDevice device;
    VkSwapchainKHR swapchain;

    uint32_t scImageCount;
    // TODO: Suballocation from main memory
    VkImage swapchainImages[10];

    int graphicsIdx;
};

bool vk_init(VkContext *vkcontext, void *window) {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.pEngineName = "Triangle Engine";
    
    char* extensions[] = {
#ifdef WINDOWS_BUILD
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif // Different OS
#endif
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

    char* layers[] = {
        "VK_LAYER_KHRONOS_validation"   // Cant remember the MACRO for the validation layer
    };

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.ppEnabledExtensionNames = extensions;
    instanceInfo.enabledExtensionCount = ArraySize(extensions);
    instanceInfo.ppEnabledLayerNames = layers;
    instanceInfo.enabledLayerCount = ArraySize(layers);

    VK_CHECK(vkCreateInstance(&instanceInfo, 0, &vkcontext->instance));

    // Debug utils messanger
    // A debug utils messenger is a way to get debug messages from the Vulkan API
    auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkcontext->instance, "vkCreateDebugUtilsMessengerEXT");
    if (vkCreateDebugUtilsMessengerEXT) {
        VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
        debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugInfo.pfnUserCallback = vkDebugCallback;

        VK_CHECK(vkCreateDebugUtilsMessengerEXT(vkcontext->instance, &debugInfo, 0, &vkcontext->debugMessenger));
    } else {
        std::cout << "Debug utils messenger not available" << std::endl;
        return false;
    }

    // Create surface
    // A surface is a connection to the windowing system
    {
#ifdef WINDOWS_BUILD
    VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hwnd = (HWND)window;
    surfaceInfo.hinstance = GetModuleHandleA(0);
    VK_CHECK(vkCreateWin32SurfaceKHR(vkcontext->instance, &surfaceInfo, 0, &vkcontext->surface));
#elif // Different OS
#endif
    }

    // Choose GPU
    // A GPU is a physical device that can be used to render graphics
    {
        vkcontext->graphicsIdx = -1;
        uint32_t gpuCount = 0;
        // TODO: Suballocation from main memory
        VkPhysicalDevice gpus[10];
        VK_CHECK(vkEnumeratePhysicalDevices(vkcontext->instance, &gpuCount, 0));
        VK_CHECK(vkEnumeratePhysicalDevices(vkcontext->instance, &gpuCount, gpus));

        for (uint32_t i = 0; i < gpuCount; i++) {
            uint32_t queueCount = 0;
            // TODO: Suballocation from main memory
            VkQueueFamilyProperties queueProperties[10];

            vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queueCount, 0);
            vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queueCount, queueProperties);

            for (uint32_t j = 0; j < queueCount; j++) {
                if (queueProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    // Found a GPU with graphics queue
                    VkBool32 surfaceSupport = VK_FALSE;
                    VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(gpus[i], j, vkcontext->surface, &surfaceSupport));

                    if (surfaceSupport) {
                        vkcontext->graphicsIdx = j;
                        vkcontext->gpu = gpus[i];
                        break;
                    }
                }
            }
        }

        if (vkcontext->graphicsIdx < 0) {
            std::cout << "No GPU with graphics queue found" << std::endl;
            return false;
        }
    }

    // Logical Device
    // A logical device is a connection to the physical device
    {
        float queuePriority = 1.0f;
        
        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = vkcontext->graphicsIdx;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        char *extensions[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        
        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.pQueueCreateInfos = &queueInfo;
        deviceInfo.queueCreateInfoCount = 1;
        deviceInfo.ppEnabledExtensionNames = extensions;
        deviceInfo.enabledExtensionCount = ArraySize(extensions);

        VK_CHECK(vkCreateDevice(vkcontext->gpu, &deviceInfo, 0, &vkcontext->device));
    }

    // Swapchain
    // A swapchain is a queue of images that are waiting to be presented to the screen
    {
        uint32_t formatCount = 0;
        // TODO: Suballocation from main memory
        VkSurfaceFormatKHR surfaceFormats[10];
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->gpu, vkcontext->surface, &formatCount, 0));
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->gpu, vkcontext->surface, &formatCount, surfaceFormats));

        for (uint32_t i = 0; i < formatCount; i++) {
            if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {
                vkcontext->surfaceFormat = surfaceFormats[i];
                break;
            }
        }
        
        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkcontext->gpu, vkcontext->surface, &surfaceCapabilities));
        uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
        imageCount > surfaceCapabilities.maxImageCount ? imageCount - 1 : imageCount;
        
        VkSwapchainCreateInfoKHR scInfo = {};
        scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;  // This makes the alpha channel ignored. Change later?
        scInfo.surface = vkcontext->surface;
        scInfo.imageFormat = vkcontext->surfaceFormat.format;
        scInfo.preTransform = surfaceCapabilities.currentTransform;
        scInfo.imageExtent = surfaceCapabilities.currentExtent;
        scInfo.minImageCount = imageCount;
        scInfo.imageArrayLayers = 1;

        VK_CHECK(vkCreateSwapchainKHR(vkcontext->device, &scInfo, 0, &vkcontext->swapchain));

        VK_CHECK(vkGetSwapchainImagesKHR(vkcontext->device, vkcontext->swapchain, &vkcontext->scImageCount, 0));
        VK_CHECK(vkGetSwapchainImagesKHR(vkcontext->device, vkcontext->swapchain, &vkcontext->scImageCount, vkcontext->swapchainImages));
    }

    return true;
}