#include "../include/worldGenerator.h"
#include "../include/chunk.h"
#include "../include/misc.h"

#include <functional>
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
    // generates block_array then passes it to chunk constructor for lighting and everything else

    std::vector<float> sizes = {100.0f, 50.0f, 10.0f};
    std::vector<float> amplitudes = {40.0f, 15.0f, 2.0f};

    Array3D block_array;
    //init chunk here
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int z=0; z<CH_WIDTH; z++)
        {
            float perlin = (int) simplexOctaves2D(x+chunk_pos.x*CH_WIDTH, z+chunk_pos.y*CH_WIDTH, sizes, amplitudes);
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


float WorldGenerator::simplexNoise2D(float x, float y, float amplitude) //"world" x and y, can be scaled
{
    float noise = 0.0f;

    //scewed coordinates 
    float NNN = 2.0f;
    float F = (std::sqrt(NNN+1)-1)/NNN;
    float G = (1-1/std::sqrt(NNN+1))/NNN;
    float x_scew = x + (x+y)*F;
    float y_scew = y + (x+y)*F;

    //which hypercube coordinates 
    float x_b = std::floor(x_scew);
    float y_b = std::floor(y_scew);

    //internal coordinates
    float x_i = x_scew-x_b;
    float y_i = y_scew-y_b;

    //order for edge traversal
    glm::ivec2 corner = glm::ivec2(x_b, y_b);
    std::vector<glm::ivec2> corners = {corner};
    if(x_i > y_i)
        corners.push_back(corner + glm::ivec2(1, 0));
    else
        corners.push_back(corner + glm::ivec2(0, 1));
    corners.push_back(corner + glm::ivec2(1, 1));

    //traverse every edge
    for (glm::ivec2& vert : corners)
    {   
        float factor = (vert.x+vert.y)*G;
        float x_n = x-(vert.x - factor);
        float y_n = y-(vert.y - factor);

        float max = std::max(0.0f, 0.5f - x_n*x_n - y_n*y_n);
        noise += max*max*max*max * glm::dot(gradients[random(vert)], glm::vec2(x_n, y_n));
    }

    //scale to [0, amplitude]
    return (70.0f * noise +1.0f)/2.0f * amplitude;
}


float WorldGenerator::simplexOctaves2D(int world_x, int world_z, std::vector<float>& sizes, std::vector<float>& amplitudes)
{
    float noise = 0.0f;

    for (int i=0; i<sizes.size(); i++)
    {
        float amplitude = amplitudes[i];
        float size = sizes[i];
        noise += simplexNoise2D(world_x/size, world_z/size, amplitude);
    }

    //bounds checking
    if (noise > CH_HEIGHT-1)
        noise = CH_HEIGHT-1;
    else if (noise < 0)
        noise = 0;
    return noise;
}


int WorldGenerator::random(glm::vec2 pos) //TODO integrate seed
{
    std::lock_guard<std::mutex> lock(rand_mutex);
    srand((((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0))*23));
    return rand() % 4;
}