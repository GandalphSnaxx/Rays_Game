/**
 * @file game.hpp
 * @author Ray Richter
 * @brief Main game header file
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef GAME_HPP
#define GAME_HPP

// #include <vk_engine.hpp>
#include "../engine/vk_engine.hpp"
#include <FlagField.hpp>
#include "requirements/requirements.hpp"

class RaysGame {
public:

    bool init();
    bool run();
    bool close();

private:
    typedef enum GameStateFlags_ {
        INITALIZED, CLOSED,
        MINIMIZED,
        RESIZE_REQUESTED,
        SHUTDOWN_REQUESTED,
        MAX
    } GameStateFlags_;
    
    FlagField<MAX, GameStateFlags_> gameState_;
    vk::Engine engine_;

    Requirements gameWinRequirement = { .description = "Meeting this requirement wins the game!" };

    void BuildRequirements_();
};

#endif // GAME_HPP