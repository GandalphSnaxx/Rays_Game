/**
 * @file camera.hpp
 * @author Ray Richter
 * @brief Camera object.
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "SDL3/SDL_events.h"
#include "vk_types.hpp"

class Camera {
public:
    glm::vec3 velocity;
    glm::vec3 position;
    glm::vec3 rot_vel;
    glm::vec3 rotation;

    glm::mat4 getViewMatrix() const;
    glm::mat4 getRotationMatrix() const;

    void processSDLEvent(SDL_Event& e);
    void update();
};

#endif // CAMERA_HPP