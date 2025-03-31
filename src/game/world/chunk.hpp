/**
 * @file chunk.hpp
 * @author Ray Richter
 * @brief World chunk
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <FlagField.hpp>

typedef enum ChunkFlags {
    LOADED, LOAD, UNLOAD,   // Is the chunk loaded, should it load or unload
    HAS_STRUCTURE,          // Does the chunk have any structure
    HAS_ORE,                // Does the chunk contain ore?
    MAX
} ChunkFlags;

struct ChunkFile {
    std::string path;
};

struct Chunk {
    // Chunk coords
    size_t x, y;

    // // Chunk mesh data
    // // Make a new struct?
    // size_t vertices_size;
    // std::vector<Vertex> vertices;
    // size_t indices_size;
    // std::vector<Index>  indices;

    // // Objects in this chunk
    // size_t objects_size;
    // std::vector<Object> objects;

    // // Chunk tag flags
    // FlagField<MAX, ChunkFlags> flags;

    // GameReturn_t load(std::string& path);
    // GameReturn_t save(std::string& path) const;
};

#endif // CHUNK_HPP