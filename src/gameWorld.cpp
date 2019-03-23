#include "../include/gameWorld.h"
#include "../include/worldGenerator.h"

#include <iostream>
#include <cmath>

GameWorld::GameWorld(WorldGenerator* p_world_gen)
{
    this->sun_direction = glm::vec3(0.2f, -1.0f, 0.5f);
    this->p_world_gen = p_world_gen;

    //code for chunk creaton here

    for(int x=-1; x<5; x++)
    {
        for (int z=-1; z<5; z++)
        {
            chunks[glm::ivec2(x, z)] = p_world_gen->generateChunk(glm::ivec2(x, z));
        }
    }
}


bool GameWorld::isInBounds(float world_x, float world_y, float world_z)
{
    int chunk_x = std::floor(world_x/Chunk::WIDTH);
    int chunk_z = std::floor(world_z/Chunk::BREADTH);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (world_y>=0 && world_y<Chunk::HEIGHT)
        if(chunks.find(pos)!=chunks.end())
            return true;
    return false;

}


void GameWorld::updateVisible(float world_x, float world_y, float world_z, bool visible)
{
    if(isInBounds(world_x, world_y, world_z))
    {
        BlockInfo& info = getBlockInfo(world_x, world_y, world_z);
        if (info.blockID!=0)
        {
        int chunk_x = std::floor(world_x/Chunk::WIDTH);
        int chunk_z = std::floor(world_z/Chunk::BREADTH);
        int local_x = std::floor(world_x) - chunk_x*Chunk::WIDTH; //might be wrong with negatives, but i think its right
        int local_y = std::floor(world_y);
        int local_z = std::floor(world_z) - chunk_z*Chunk::BREADTH;
        chunks[glm::ivec2(chunk_x, chunk_z)].block_locations[info.blockID].insert(glm::ivec3(local_x, local_y, local_z));
        info.visible = visible;
        }
    }
}


void GameWorld::removeBlock(float world_x, float world_y, float world_z)
{
    if (isInBounds(world_x, world_y, world_z))
    {
        BlockInfo& info = getBlockInfo(world_x, world_y, world_z);
        int chunk_x = std::floor(world_x/Chunk::WIDTH);
        int chunk_z = std::floor(world_z/Chunk::BREADTH);
        int local_x = std::floor(world_x) - chunk_x*Chunk::WIDTH; //might be wrong with negatives, but i think its right
        int local_y = std::floor(world_y);
        int local_z = std::floor(world_z) - chunk_z*Chunk::BREADTH;
        chunks[glm::ivec2(chunk_x, chunk_z)].block_locations[info.blockID].erase(glm::ivec3(local_x, local_y, local_z));
        info.blockID=0;
        info.visible=false;
        updateVisible(world_x+1, world_y, world_z, true);
        updateVisible(world_x-1, world_y, world_z, true);
        updateVisible(world_x, world_y+1, world_z, true);
        updateVisible(world_x, world_y-1, world_z, true);
        updateVisible(world_x, world_y, world_z+1, true);
        updateVisible(world_x, world_y, world_z-1, true);
    }
}


BlockInfo& GameWorld::getBlockInfo(float world_x, float world_y, float world_z)
{
    //WARNING check if in bounds outside of this function
    int chunk_x = std::floor(world_x/Chunk::WIDTH);
    int chunk_z = std::floor(world_z/Chunk::BREADTH);
    int local_x = std::floor(world_x) - chunk_x*Chunk::WIDTH; //might be wrong with negatives, but i think its right
    int local_y = std::floor(world_y);
    int local_z = std::floor(world_z) - chunk_z*Chunk::BREADTH;
    BlockInfo& info = chunks[glm::vec2(chunk_x, chunk_z)].getBlockInfo(local_x, local_y, local_z);
    return info;
}