#include "../include/gameWorld.h"
#include "../include/worldGenerator.h"
#include "../include/misc.h"

#include <iostream>
#include <cmath>

GameWorld::GameWorld() : player(Player())
{
    this->sun_direction = glm::vec3(1, -4, 2);
    this->block_is_targeted = false;
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


bool GameWorld::isInBounds(float x, float y, float z)
{
    return chunk_manager.isInBounds(x, y, z);
}


bool GameWorld::isInBounds(int x, int y, int z)
{
    return chunk_manager.isInBounds(x, y, z);
}


void GameWorld::changeBlock(float x, float y, float z, int blockID)
{
    chunk_manager.changeBlock(x, y, z, blockID);
}


void GameWorld::changeBlock(int x, int y, int z, int blockID)
{
    chunk_manager.changeBlock(x, y, z, blockID);
}


BlockInfo& GameWorld::getBlockInfo(float x, float y, float z)
{
    return chunk_manager.getBlockInfo(x,y,z);
}


BlockInfo& GameWorld::getBlockInfo(int x, int y, int z)
{
    return chunk_manager.getBlockInfo(x,y,z);
}


glm::mat4 GameWorld::getViewMatrix()
{
    return player.camera.getViewMatrix();
}