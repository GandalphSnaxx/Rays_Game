/**
 * @file tags.hpp
 * @author Ray Richter
 * @brief Tag management
 * @version 0.1
 * @date 2025-04-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef TAGS_HPP
#define TAGS_HPP

#include <string>
#include <vector>

namespace game {

struct Id {
    std::string space;
    std::string name;
};

struct Tag {
    std::string title;
    std::vector<Id> list;
};

}; // namespace game

#endif // TAGS_HPP