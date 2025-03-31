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

bool RaysGame::init() {
    engine_.init();

    return false;
}
bool RaysGame::run() {
    engine_.draw();

    return false;
}
bool RaysGame::close() {
    engine_.clean();

    return false;
}

void RaysGame::BuildRequirements_() {
    
};