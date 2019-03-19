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
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> chunks;

    GameWorld(WorldGenerator* p_world_gen);
    void removeBlock(float world_x, float world_y, float world_z);
    BlockInfo& getBlockInfo(float world_x, float world_y, float world_z);
    bool isInBounds(float world_x, float world_y, float world_z);
    void updateVisible(float world_x, float world_y, float world_z, bool visible);
private:
    WorldGenerator* p_world_gen;
};

#endif // GAMEWORLD_H