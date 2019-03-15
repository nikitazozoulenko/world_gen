#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "block.h"
#include "worldGenerator.h"

#include <vector>
class GameWorld
{
public:
    glm::vec3 sun_direction;
    std::vector<Chunk> chunks;

    GameWorld(WorldGenerator* p_world_gen);
private:
    WorldGenerator* p_world_gen;
};

#endif // GAMEWORLD_H