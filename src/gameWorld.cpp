#include "../include/gameWorld.h"
#include "../include/worldGenerator.h"

#include <iostream>
#include <cmath>

GameWorld::GameWorld(WorldGenerator* p_world_gen) : player(Player())
{
    this->sun_direction = glm::vec3(0.2f, -1.0f, 0.5f);
    this->p_world_gen = p_world_gen;
    this->block_is_targeted = false;

    //code for chunk creaton here
    for(int x=-3; x<3; x++)
    {
        for (int z=-3; z<3; z++)
        {
            chunks[glm::ivec2(x, z)] = p_world_gen->generateChunk(glm::ivec2(x, z));
        }
    }
}


bool GameWorld::isInBounds(float x, float y, float z)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    return isInBounds(x_int, y_int, z_int);
}


bool GameWorld::isInBounds(int x, int y, int z)
{
    int chunk_x = std::floor(x / (float)Chunk::WIDTH);
    int chunk_z = std::floor(y / (float) Chunk::BREADTH);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<Chunk::HEIGHT)
        if(chunks.find(pos)!=chunks.end())
            return true;
    return false;
}


void GameWorld::updateVisible(float x, float y, float z)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    updateVisible(x_int, y_int, z_int);
}


void GameWorld::updateVisible(int x, int y, int z)
{
    //TODO : WARNING : doesnt work properly on the corner of a chunk when the nearby chunk is unloaded
    if(isInBounds(x, y, z))
    {
        int positions[6][3] = {{x+1,y,  z  },
                            {x,  y+1,z  },
                            {x,  y,  z+1},
                            {x-1,y,  z  },
                            {x,  y-1,z  },
                            {x,  y,  z-1}};

        //check visible status on all sides. top block of the world always visible
        bool visible = false;
        for(auto& pos : positions)
        {
            if(isInBounds(pos[0], pos[1], pos[2]))
            {
                BlockInfo& info = getBlockInfo(pos[0], pos[1], pos[2]);
                if (pos[1] == Chunk::HEIGHT-1 || info.blockID == 0)
                {
                    visible = true;
                    break;
                }
            }
        }

        //update visibility lists
        BlockInfo& info = getBlockInfo(x, y, z);
        int ch_x = std::floor(x / (float)Chunk::WIDTH);
        int ch_z = std::floor(z / (float)Chunk::BREADTH);
        int local_x = x - ch_x*Chunk::WIDTH;
        int local_y = y;
        int local_z = z - ch_z*Chunk::BREADTH;
        chunks[glm::ivec2(ch_x, ch_z)].block_locations[info.blockID].erase(glm::vec3(local_x, local_y, local_z));
        if (visible)
            chunks[glm::ivec2(ch_x, ch_z)].block_locations[info.blockID].insert(glm::vec3(local_x, local_y, local_z));
    }
}

void GameWorld::changeBlock(float x, float y, float z, int blockID)
{
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    changeBlock(x_int, y_int, z_int, blockID);
}


void GameWorld::changeBlock(int x, int y, int z, int blockID)
{
    if (isInBounds(x, y, z))
    {
        BlockInfo& info = getBlockInfo(x, y, z);
        int ch_x = std::floor(x / (float)Chunk::WIDTH);
        int ch_z = std::floor(z / (float)Chunk::BREADTH);
        int local_x = x - ch_x*Chunk::WIDTH;
        int local_y = y;
        int local_z = z - ch_z*Chunk::BREADTH;
        chunks[glm::ivec2(ch_x, ch_z)].block_locations[info.blockID].erase(glm::vec3(local_x, local_y, local_z));
        info.blockID=blockID;
        updateVisible(x, y, z);
        updateVisible(x+1, y, z);
        updateVisible(x-1, y, z);
        updateVisible(x, y+1, z);
        updateVisible(x, y-1, z);
        updateVisible(x, y, z+1);
        updateVisible(x, y, z-1);
    }
}


BlockInfo& GameWorld::getBlockInfo(float x, float y, float z)
{
    //WARNING check if in bounds outside of this function
    int x_int = std::floor(x);
    int y_int = std::floor(y);
    int z_int = std::floor(z);
    return getBlockInfo(x_int,y_int,z_int);
}


BlockInfo& GameWorld::getBlockInfo(int x, int y, int z)
{
    //WARNING check if in bounds outside of this function
    int chunk_x = std::floor(x / (float)Chunk::WIDTH);
    int chunk_z = std::floor(z / (float)Chunk::BREADTH);
    int local_x = x - chunk_x*Chunk::WIDTH;
    int local_y = y;
    int local_z = z - chunk_z*Chunk::BREADTH;
    BlockInfo& info = chunks[glm::ivec2(chunk_x, chunk_z)].getBlockInfo(local_x, local_y, local_z);
    return info;
}


glm::mat4 GameWorld::getViewMatrix()
{
    return player.camera.getViewMatrix();
}


void GameWorld::updateTarget()
{
    targetBlockRay(player.pos.x, player.pos.y, player.pos.z, glm::vec3(0,0,0));
}


void GameWorld::targetBlockRay(float x, float y, float z, glm::vec3 previous_block_pos)
{
    block_is_targeted = false;
    float dir_x = player.camera.front.x;
    float dir_y = player.camera.front.y;
    float dir_z = player.camera.front.z;
    if(player.distanceTo(x,y,z) < player.reach)
    {
        glm::vec3 current_pos = glm::vec3(std::floor(x), std::floor(y), std::floor(z));  //new glm::vec3???
        if(isInBounds(x,y,z))
        {
            BlockInfo info = getBlockInfo(x,y,z);
            if(info.blockID != 0)
            {
                block_is_targeted = true;
                target_pos = current_pos;
                before_pos = previous_block_pos;
                return;
            }
        }

        //"projection" on cube sides
        float x_len = std::floor(x) - x;
        float tx = std::max(x_len/dir_x, (1+x_len)/dir_x);
        float y_len = std::floor(y) - y;
        float ty = std::max(y_len/dir_y, (1+y_len)/dir_y);
        float z_len = std::floor(z) - z;
        float tz = std::max(z_len/dir_z, (1+z_len)/dir_z);

        float t;
        float eps = 0.001;
        if(tx<ty && tx<tz)
            t=tx + eps;
        else if(ty<tx && ty<tz)
            t=ty + eps;
        else
            t=tz + eps;

        targetBlockRay(x+t*dir_x, y+t*dir_y, z+t*dir_z, current_pos);
    }
}