/**
 * @file game.cpp
 * @author Ray Richter
 * @brief Game definitions
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "game.hpp"
#include <iostream>

bool RaysGame::init() {
    VkInit initState = { .appName = "Rays Game", .defaultWindowSize = { 800, 600 }, .validationLayersEnable = true };
    if (!engine_.init(initState)) {
        std::cout << "Engine initalization failed!" << std::endl;
        return true;
    }

    gameState_ += INITALIZED;
    return false;
}
bool RaysGame::run() {
    if (!gameState_(INITALIZED)) {
        std::cout << "Game is not initalized, cannot run!" << std::endl;
        return true;
    }

    while (!engine_.should_close()) {
        for (SDL_Event event; SDL_PollEvent(&event);) {
            engine_.draw(event);
        }
    }

    return false;
}
bool RaysGame::close() {
    if (gameState_(CLOSED)) {
        std::cout << "Game is already closed!" << std::endl;
        return false;
    }
    if (!gameState_(INITALIZED)) {
        std::cout << "Game is not initalized, cannot close!" << std::endl;
        return true;
    }
    engine_.clean();

    return false;
}

void RaysGame::BuildRequirements_() {
    
};