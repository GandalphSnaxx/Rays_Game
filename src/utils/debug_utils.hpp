/**
 * @file debug_utils.hpp
 * @author Ray Richter
 * @brief Debugging utilities
 * @version 0.1
 * @date 2025-04-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef DEBUG_UTILS_HPP
#define DEBUG_UTILS_HPP

#include <iostream>
#include <string>

/// @brief Defines in reference to an object (or cpp file)
// #define DEBUG_DEFINE(object)    std::cout << "[" << object << "] "
#define DEBUG_HEAD(head)    std::string msg = head
#define DEBUG_MSG(message)  std::cout << msg << " Success\n\t" << message << std::endl
#define DEBUG_ERR(message)  std::cout << msg << " !Error!\n\t" << message << std::endl

#endif // DEBUG_UTILS_HPP