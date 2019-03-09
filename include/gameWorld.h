#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "block.h"

#include <vector>
class GameWorld
{
public:
    glm::vec3 sun_direction;
    std::vector<Chunk> chunks;
    //WorldGenerator world_gen

    GameWorld();
private:
};

#endif // GAMEWORLD_H