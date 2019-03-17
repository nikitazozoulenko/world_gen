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
    //generate a bit larger chunk for visibility checking
    Array3D<2+Chunk::WIDTH, Chunk::HEIGHT, 2+Chunk::BREADTH> block_array;
    //init chunk here genChunk(position)
    for (int x=0; x<2+Chunk::WIDTH; x++)
    {
        for (int z=0; z<2+Chunk::BREADTH; z++)
        {
            float perlin = (int) totalPerlinValue((x-1)+chunk_pos.x*Chunk::WIDTH, (z-1)+chunk_pos.y*Chunk::BREADTH);
            for (int y=0; y<Chunk::HEIGHT; y++)
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
    //addPlayerPlacedBlocks();

    Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::BREADTH> final_block_array;
    visibiltyChecking(block_array, final_block_array);
    return Chunk(chunk_pos, final_block_array);
}


float WorldGenerator::totalPerlinValue(int world_x, int world_z)
{
    float total_perlin = 0;

    std::vector<int> sizes = {100, 50, 8};
    std::vector<float> amplitudes = {50.0f, 15.0f, 2.0f};
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
    srand((((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0))*23));
    return rand() % 4;
}


void WorldGenerator::visibiltyChecking(Array3D<2+Chunk::WIDTH, Chunk::HEIGHT, 2+Chunk::BREADTH>& block_array, Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::BREADTH>& final_block_array)
{
    //visibility: highest y always seen, lowest y check 5 blocks, rest check 6 blocks
    for (int x=0; x<Chunk::WIDTH; x++)
        for (int y=0; y<Chunk::HEIGHT; y++)
            for (int z=0; z<Chunk::BREADTH; z++)
            {
                BlockInfo& block_info = block_array.at(x+1, y, z+1);
                if (block_info.blockID != 0)
                {
                    //check all 4 allowed spots
                    bool north = (bool) block_array.at(x+2, y, z+1).blockID;
                    bool south = (bool) block_array.at(x, y, z+1).blockID;
                    bool east = (bool) block_array.at(x+1, y, z+2).blockID;
                    bool west = (bool) block_array.at(x+1, y, z).blockID;

                    block_info.visible = true;
                    if (y == Chunk::HEIGHT-1)
                        block_info.visible = true;
                    else // top now allowed
                    {
                        bool top = (bool) block_array.at(x+1, y+1, z+1).blockID;
                        if (y==0)
                        {
                            if (north && south && east && west && top) 
                                block_info.visible = false;
                        }
                        else //and finally if y is in the middle, 6 allowed
                        {
                            bool bottom = (bool) block_array.at(x+1, y-1, z+1).blockID;
                            if (north && south && east && west && top && bottom)
                                block_info.visible = false;
                        }
                    }
                }
                final_block_array.at(x, y, z) = block_array.at(x+1,y,z+1);
            }
}