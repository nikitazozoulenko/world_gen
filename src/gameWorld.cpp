#include "../include/gameWorld.h"
#include "../include/worldGenerator.h"

#include <iostream>

GameWorld::GameWorld()
{
    sun_direction = glm::vec3(0.2f, -1.0f, 0.5f);

    //code for chunk creaton here
    WorldGenerator world_gen = WorldGenerator(123456);


    for(int x=0; x<7; x++)
    {
        for (int z=0; z<7; z++)
        {
            chunks.push_back(world_gen.generateChunk(glm::vec2(x, z)));
        }
    }
}