#ifndef VULKAN_SURFACE_HPP__
#define VULKAN_SURFACE_HPP__

#include "overhead.hpp"
#include <vector>

/// @brief VkSurfaceClass is a child class of VkOverheadClass. Contains functions related to a VkSurface
///     Requires access to _surface
class VkSurfaceClass : virtual public VkOverheadClass {
    public:

    VkSurfaceClass();
    ~VkSurfaceClass();
    // VkSurfaceClass(const WindowInit *window_init, const char *appName);
    // Delete the default constructor to prevent its use
    // VkSurfaceClass::VkSurfaceClass() = delete;

    protected:

    private:

};

#endif