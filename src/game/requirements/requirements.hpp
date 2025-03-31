/**
 * @file requirements.hpp
 * @author Ray Richter
 * @brief Requirements struct
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef REQUIREMENTS_HPP_
#define REQUIREMENTS_HPP_

#include <string>
#include <vector>

typedef enum RequirementID_t {
    REQUIREMENT_ID_REPAIR_GATEWAY,
    REQUIREMENT_ID_CONQUOR_SOLAR_SYSTEM,
    REQUIREMENT_ID_GAME_END,
    REQUIREMENT_ID0, REQUIREMENT_ID1, REQUIREMENT_ID2,// ...
    REQUIREMENT_ID_NULL,
    MAX_REQUIREMENT_ID_ENUM
} RequirementID_t;

class RequirementDesc { public:
    std::string title = "";
    std::string description = "";
    size_t id = MAX_REQUIREMENT_ID_ENUM;
};

class Requirements { public:
    RequirementDesc description;
    size_t level;

    std::vector<Requirements>       subRequirements; // Requirements that must be met in order to complete this
    std::vector<RequirementDesc>    subRequirementDescriptions; // Provide context as to how they apply to this

    // void addRequirement(const Requirements* requirement);
};

// Prebuilt requirements
const RequirementDesc REQUIREMENT_DESC_REPAIR_GATEWAY = {
    .title = "Repair the Mysterious Gateway",
    .description = "Repair the mysterious gateway and use it to travel to another solar system",
    .id = REQUIREMENT_ID_REPAIR_GATEWAY
};
const Requirements REQUIREMENT_REPAIR_GATEWAY = {
    .description = REQUIREMENT_DESC_REPAIR_GATEWAY,
    .level = 20
};

const RequirementDesc REQUIREMENT_DESC_CONQUOR_SOLAR_SYSTEM = { 
    .title = "Conquor Solar System", 
    .description = "Craft the most valuable resource of each planet in the solar system",
    .id = REQUIREMENT_ID_CONQUOR_SOLAR_SYSTEM
};
const Requirements REQUIREMENT_CONQUOR_SOLAR_SYSTEM = { .description = REQUIREMENT_DESC_CONQUOR_SOLAR_SYSTEM, .level = 10 };

const RequirementDesc REQUIREMENT_DESC_GAME_END = { .title = "Game End", .description = "Beating this requirement wins the game.", .id = REQUIREMENT_ID_GAME_END };
const Requirements REQUIREMENT_GAME_END = { .description = REQUIREMENT_DESC_GAME_END, .level = 100,
    .subRequirements = { REQUIREMENT_CONQUOR_SOLAR_SYSTEM, REQUIREMENT_REPAIR_GATEWAY },
};

#endif // REQUIREMENTS_HPP