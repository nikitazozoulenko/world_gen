#include "../include/worldGenerator.h"   

#include <iostream>
#include <cstdlib>
#include <cmath>


WorldGenerator::WorldGenerator(int seed) //TODO seed
{
    this->seed = seed;
    gradients[0] = glm::vec2(-1.0, 1.0);
    gradients[1] = glm::vec2(1.0, 1.0);
    gradients[2] = glm::vec2(-1.0, 1.0);
    gradients[3] = glm::vec2(-1.0, -1.0);
}


Chunk WorldGenerator::generateChunk(glm::vec2 chunk_pos)
{
    Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::BREADTH> block_array;
    //init chunk here genChunk(position)
    for (int x=0; x<Chunk::WIDTH; x++)
    {
        for (int z=0; z<Chunk::BREADTH; z++)
        {
            float perlin = totalPerlinValue(x+chunk_pos.x*Chunk::WIDTH, z+chunk_pos.y*Chunk::BREADTH);
            block_array.at(x, (int) perlin, z).blockID = 3;
        }
    }
    return Chunk(chunk_pos, block_array);
}


float WorldGenerator::totalPerlinValue(int world_x, int world_z)
{
    float total_perlin = 0;

    std::vector<int> sizes = {128, 64};
    std::vector<int> amplitudes = {32, 16};
    for (int i=0; i<sizes.size(); i++)
    {
        int width = sizes[i];
        int amplitude = amplitudes[i];

        int x_corner = world_x/width;
        int z_corner = world_z/width;
        float x_norm = std::fmod(world_x+0.5f, width)/(float)width;
        float z_norm = std::fmod(world_z+0.5f, width)/(float)width;

        //negative checking
        if (x_norm < 0)
            x_norm += 1;
        if (z_norm < 0)
            z_norm += 1;
        if (world_x < 0)
            x_corner -= 1;
        if (world_z < 0)
            z_corner -= 1;
        if (world_x % width == 0 && world_x < 0)
            x_norm = 1-x_norm;
        if (world_z % width == 0 && world_z < 0)
            z_norm = 1-z_norm;

        total_perlin += getPerlinValue(x_norm, z_norm, glm::vec2(x_corner, z_corner), amplitude);
    }

    //bounds checking
    if (total_perlin > Chunk::HEIGHT-1)
        total_perlin = Chunk::HEIGHT-1;
    else if (total_perlin < 0)
        total_perlin = 0;
    return total_perlin;
}


float WorldGenerator::lerp(float val0, float val1, float t)
{
    return val0*t + (1-t)*val1;
}


float WorldGenerator::getPerlinValue(float x, float y, glm::vec2 corner_pos, int amplitude)
{   
    //get each corner vector
    glm::vec2 grad00 = gradients[random(corner_pos)];
    glm::vec2 grad10 = gradients[random(corner_pos+glm::vec2(-1,0))];
    glm::vec2 grad11 = gradients[random(corner_pos+glm::vec2(-1,-1))];
    glm::vec2 grad01 = gradients[random(corner_pos+glm::vec2(0,-1))];

    //dot product with position vector from corner
    float dot00 = grad00.x*x + grad00.y*y;
    float dot01 = grad01.x*x + grad01.y*(1-y);
    float dot10 = grad10.x*(1-x) + grad10.y*y;
    float dot11 = grad11.x*(1-x) + grad11.y*(1-y);

    //interpolation
    x = x*x*x*(x * ((x * 6) - 15) + 10); ///6t^5 - 15t^4 + 10t^3
    y = y*y*y*(y * ((y * 6) - 15) + 10);
    float lerpx0 = lerp(dot00, dot01, y);
    float lerpx1 = lerp(dot10, dot11, y);
    float perlin = lerp(lerpx0, lerpx1, x); //from -2 to 2 i think

    return perlin*amplitude/2.0f + amplitude;
}


int WorldGenerator::random(glm::vec2 pos) //TODO integrate seed
{
    srand((((((pos.x)*29 + 11)*13)+17 + 27*pos.y)*23));
    return rand() % 4;
}