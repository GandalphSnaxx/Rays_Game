/**
 * @file vk_engine.hpp
 * @author Ray Richter
 * @brief Vulkan engine
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

namespace vk {

class Engine {
public:

private:
    typedef enum EngineStateFlags_ {
        INITALIZED,
        MINIMIZED,
        RESIZE_REQUESTED,
        SHUTDOWN_REQUESTED,
        MAX
    } EngineStateFlags_;
    FlagField<EngineStateFlags_, MAX> flags_;
};

} // namespace vk