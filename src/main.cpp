/**
 * @file main.cpp
 * @author Ray Richter
 * @brief Main cpp file
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "game/game.hpp"
#include <fmt/core.h>

int main(const int argc, const void* argv) {
    RaysGame game;

    game.init();

    game.run();

    game.close();

    fmt::print("Hello World!\n");

    return 0;
}