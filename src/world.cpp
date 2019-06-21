#include "../include/world.h"
#include "../include/misc.h"

#include <iostream>
#include <cmath>

World::World()
{
    for(int x=0; x<5; x++)
    {
        for(int z=0; z<5; z++)
        {   
            glm::ivec2 pos(x,z);
            chunk_map[pos] = Chunk(pos);
        }
    }
}


std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>>& World::getChunkMap()
{
    return chunk_map;
}