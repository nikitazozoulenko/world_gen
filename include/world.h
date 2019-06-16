#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "chunk.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class World
{
public:
    World();
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>>& getChunkMap();
private:
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> chunk_map;
};

#endif // GAMEWORLD_H