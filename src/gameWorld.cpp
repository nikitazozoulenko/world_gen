#include "../include/gameWorld.h"

#include <iostream>

GameWorld::GameWorld()
{
    sun_direction = glm::vec3(0.2f, -1.0f, 0.5f);

    for(int x=0; x<3; x++)
    {
        for (int z=0; z<3; z++)
        {
            chunks.push_back(Chunk(glm::vec2(x,z)));
        }
    }
}