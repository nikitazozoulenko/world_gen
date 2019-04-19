#include "../include/worldGenerator.h"
#include "../include/chunk.h"
#include "../include/misc.h"

#include <iostream>
#include <cstdlib>
#include <cmath>


WorldGenerator::WorldGenerator(int seed) ://TODO seed
    seed(seed)
{
    gradients[0] = glm::vec2(-1.0, 1.0);
    gradients[1] = glm::vec2(1.0, 1.0);
    gradients[2] = glm::vec2(-1.0, 1.0);
    gradients[3] = glm::vec2(-1.0, -1.0);
}

Chunk WorldGenerator::generateChunk(glm::vec2 chunk_pos) 
{   
    //? generates block_array then passes it to chunk constructor for lighting

    Array3D block_array;
    //init chunk here
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int z=0; z<CH_WIDTH; z++)
        {
            float perlin = (int) totalPerlinValue(x+chunk_pos.x*CH_WIDTH, z+chunk_pos.y*CH_WIDTH);
            for (int y=0; y<CH_HEIGHT; y++)
            {
                unsigned int blockID = 0;
                if(y==perlin)
                    blockID = 2; //grass
                else if (y<perlin && y>perlin-4)
                    blockID = 1; //dirt
                else if (y<= perlin-4)
                    blockID = 3; //stone
                block_array.at(x, y, z).blockID = blockID;
            }
        }
    }

    Chunk chunk = Chunk(chunk_pos, block_array);
    return chunk;
}


float WorldGenerator::totalPerlinValue(int world_x, int world_z)
{
    float total_perlin = 0;

    std::vector<float> sizes = {100.0f, 50.0f, 10.0f};
    std::vector<float> amplitudes = {40.0f, 15.0f, 2.0f};
    for (int i=0; i<sizes.size(); i++)
    {
        float width = sizes[i];
        float amplitude = amplitudes[i];

        int x_corner = std::floor(world_x / width);
        int z_corner = std::floor(world_z / width);

        int local_x = std::floor(world_x) - x_corner*width;
        int local_z = std::floor(world_z) - z_corner*width;
        float x_norm = (local_x+0.5f) / width;
        float z_norm = (local_z+0.5f) / width;

        total_perlin += getPerlinValue(x_norm, z_norm, glm::vec2(x_corner, z_corner), amplitude);
    }

    //bounds checking
    if (total_perlin > CH_HEIGHT-1)
        total_perlin = CH_HEIGHT-1;
    else if (total_perlin < 0)
        total_perlin = 0;
    return total_perlin;
}


float WorldGenerator::lerp(float val0, float val1, float t)
{
    return (1-t)*val0 + val1*t;
}


float WorldGenerator::getPerlinValue(float x, float y, glm::vec2 corner_pos, float amplitude)
{   
    //get each corner vector
    glm::vec2 grad00 = gradients[random(corner_pos)];
    glm::vec2 grad10 = gradients[random(corner_pos+glm::vec2(1,0))];
    glm::vec2 grad11 = gradients[random(corner_pos+glm::vec2(1,1))];
    glm::vec2 grad01 = gradients[random(corner_pos+glm::vec2(0,1))];

    //dot product with position vector from corner
    float dot00 = grad00.x*x + grad00.y*y;
    float dot01 = grad01.x*x + grad01.y*(y-1);
    float dot10 = grad10.x*(x-1) + grad10.y*y;
    float dot11 = grad11.x*(x-1) + grad11.y*(y-1);

    //interpolation
    x = x*x*x*(x * ((x * 6) - 15) + 10); ///6t^5 - 15t^4 + 10t^3
    y = y*y*y*(y * ((y * 6) - 15) + 10);
    float lerpx0 = lerp(dot00, dot01, y);
    float lerpx1 = lerp(dot10, dot11, y);
    float perlin = lerp(lerpx0, lerpx1, x);  // from approx -0.813 to 0.813

    return perlin*amplitude + 0.82f*amplitude;
}


int WorldGenerator::random(glm::vec2 pos) //TODO integrate seed
{
    std::lock_guard<std::mutex> lock(rand_mutex);
    srand((((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0))*23));
    return rand() % 4;
}