cmake_minimum_required(VERSION 3.10)

# Project name
project(RaysGame VERSION 0.1.0)

# Include Vulkan
find_package(Vulkan REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(sdl3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(vk-bootstrap CONFIG REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(RaysGame ${SOURCES})
# add_executable(RaysGame src/main.cpp)

# Include directories
# include_directories(src)
# add_subdirectory(Libraries)

# Add FlagField include directory
set(FF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/FlagField")
include_directories(${FF_INCLUDE_DIR})

# Link Vulkan library
target_link_libraries(
    RaysGame PRIVATE 
    Vulkan::Vulkan
    fmt::fmt
    glm::glm
    SDL3::SDL3
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
)
