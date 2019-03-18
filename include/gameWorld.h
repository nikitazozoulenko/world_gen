#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "block.h"
#include "worldGenerator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class GameWorld
{
public:
    glm::vec3 sun_direction;
    std::unordered_map<glm::vec2, Chunk, std::hash<glm::vec2>> chunks;

    GameWorld(WorldGenerator* p_world_gen);
private:
    WorldGenerator* p_world_gen;
};

#endif // GAMEWORLD_H