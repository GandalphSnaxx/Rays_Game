/**
 * @file utils.hpp
 * @author Ray Richter
 * @brief Utilities
 * @version 0.1
 * @date 2025-03-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <fstream>
#include <string>
#include <vector>

/// @brief Reads the binary data from a file
std::vector<char> readFile(const std::string& fileName);