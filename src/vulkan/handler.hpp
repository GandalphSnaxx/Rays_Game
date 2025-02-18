#ifndef VKUKAN_HANDLER_HPP__
#define VKUKAN_HANDLER_HPP__

// #include "overhead.hpp"
#include "window/window.hpp"
#include "instance/instance.hpp"
#include "surface/surface.hpp"
#include "devices/device.hpp"
#include "swapchain/swapchain.hpp"
#include "memory/memory_mgmt.hpp"

/// @brief A child class of every Vulkan class required to handle Vulkan
class VkHandlerClass : 
    // Every Vulkan support class required by the handler
    virtual public VkUBOOverheadClass,
    virtual public VkSwapchainClass,
    virtual public VkDeviceClass, 
    virtual public VkSurfaceClass,
    virtual public VkInstanceClass,
    virtual public VkWindowClass
{
    public:

    VkHandlerClass(const WindowInit *window_init, const char *appName, uint32_t max_frames_in_flight);
    ~VkHandlerClass();
    // Delete the default constructor so it is not accidentally used
    VkHandlerClass::VkHandlerClass() = delete;

    private:

};

#endif