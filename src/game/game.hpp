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

typedef struct normal8_t {
public:
    normal8_t(const uint8_t x) : v(x) {}
    normal8_t() : v(0) {}

    // Examples:
    // 0b0000_0000: 0.0
    // 0b1111_1111: 1.0
    // 0b00xx_xxxx: [0.0 - 0.25)
    // 0b01xx_xxxx: [0.25 - 0.5)
    // 0b10xx_xxxx: [0.5 - 0.75)
    // 0b11xx_xxxx: [0.75 - 1.0]

    normal8_t operator+(const normal8_t& other) const {
        return normal8_t((v + other.v) / 2);
    }
private:
    uint8_t v;
} normal8_t;

#endif // GAME_HPP