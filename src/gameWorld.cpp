#include "../include/gameWorld.h"
#include "../include/worldGenerator.h"

#include <iostream>

GameWorld::GameWorld(WorldGenerator* p_world_gen)
{
    this->sun_direction = glm::vec3(0.2f, -1.0f, 0.5f);
    this->p_world_gen = p_world_gen;

    //code for chunk creaton here

    for(int x=0; x<7; x++)
    {
        for (int z=0; z<7; z++)
        {
            chunks.push_back(p_world_gen->generateChunk(glm::vec2(x, z)));
        }
    }
}