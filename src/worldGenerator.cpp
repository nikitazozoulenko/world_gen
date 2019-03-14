#include "../include/worldGenerator.h"   

#include <iostream>
    
WorldGenerator::WorldGenerator(int seed)
{
    gradients[0] = glm::vec2(-1.0, 1.0);
    gradients[1] = glm::vec2(1.0, 1.0);
    gradients[2] = glm::vec2(-1.0, 1.0);
    gradients[3] = glm::vec2(-1.0, -1.0);
}


Chunk WorldGenerator::generateChunk(glm::vec2 world_pos)
{
    Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::WIDTH> block_array;

    //init chunk here genChunk(position)
    for (int x=0; x<Chunk::WIDTH; x++)
    {
        for (int z=0; z<Chunk::WIDTH; z++)
        {
            float x_norm = (((float)x) + 0.5)/((float) Chunk::WIDTH);
            float z_norm = (((float)z) + 0.5)/((float) Chunk::WIDTH);
            float perlin = 6*getPerlinValue(x_norm, z_norm, world_pos)+14;
            block_array.at(x, (int) perlin, z).blockID = 1;                
        }
    }

    return Chunk(world_pos, block_array);
}


void WorldGenerator::setSeed(int seed)
{

}


float WorldGenerator::lerp(float val0, float val1, float t)
{
    return val0*t + (1-t)*val1;
}


float WorldGenerator::getPerlinValue(float x, float y, glm::vec2 world_pos)
{
    glm::vec2 grad00 = gradients[random(world_pos)];
    glm::vec2 grad10 = gradients[random(world_pos+glm::vec2(-1,0))];
    glm::vec2 grad11 = gradients[random(world_pos+glm::vec2(-1,-1))];
    glm::vec2 grad01 = gradients[random(world_pos+glm::vec2(0,-1))];

    float dot00 = grad00.x*x + grad00.y*y;
    float dot01 = grad01.x*x + grad01.y*(1-y);
    float dot10 = grad10.x*(1-x) + grad10.y*y;
    float dot11 = grad11.x*(1-x) + grad11.y*(1-y);

    float lerpx0 = lerp(dot00, dot01, y);
    float lerpx1 = lerp(dot10, dot11, y);
    float perlin = lerp(lerpx0, lerpx1, x); //from -2 to 2 i think
    std::cout << perlin << std::endl;

    return perlin;
}


int WorldGenerator::random(glm::vec2 pos) //DÅLIG, TODO
{
    int h = 1+pos.x*374761393 + pos.y*668265263; //all constants are prime
    h = (h^(h >> 13))*1274126177;
    return h % 4;
}