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
    gradients_noise_2D[0] = glm::vec2(-1.0, 1.0);
    gradients_noise_2D[1] = glm::vec2(1.0, 1.0);
    gradients_noise_2D[2] = glm::vec2(-1.0, 1.0);
    gradients_noise_2D[3] = glm::vec2(-1.0, -1.0);

    gradients_noise_3D[0] = glm::vec3( 1.0, -1.0, -1.0);
    gradients_noise_3D[1] = glm::vec3(-1.0,  1.0, -1.0);
    gradients_noise_3D[2] = glm::vec3(-1.0, -1.0,  1.0);
    gradients_noise_3D[3] = glm::vec3( 1.0,  1.0, -1.0);
    gradients_noise_3D[4] = glm::vec3( 1.0, -1.0,  1.0);
    gradients_noise_3D[5] = glm::vec3(-1.0,  1.0,  1.0);
    gradients_noise_3D[6] = glm::vec3( 1.0,  1.0,  1.0);
    gradients_noise_3D[7] = glm::vec3(-1.0, -1.0, -1.0);
}


Chunk WorldGenerator::generateChunk(glm::ivec2 chunk_pos) 
{   
    // generates block_array then passes it to chunk constructor for lighting and everything else

    //  mountains
    // std::vector<float> sizes = {170, 140, 30, 10};
    // std::vector<float> amplitudes = {50,40, 7, 2};

    std::vector<float> sizes = {32};
    std::vector<float> amplitudes = {20};

    Array3D block_array;
    //init chunk here
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int z=0; z<CH_WIDTH; z++)
        {
            int perlin = (int) simplexOctaves2D(x+chunk_pos.x*CH_WIDTH, z+chunk_pos.y*CH_WIDTH, sizes, amplitudes);
            for (int y=0; y<CH_HEIGHT; y++)
            {
                // unsigned int blockID = 0;
                // if(y==perlin)
                //     blockID = 2; //grass
                // else if (y<perlin && y>perlin-4)
                //     blockID = 1; //dirt
                // else if (y<= perlin-4)
                //     blockID = 3; //stone
                // block_array.at(x, y, z).blockID = blockID;
                unsigned int blockID = 0;
                if(y==perlin) //temp all is dirt
                    blockID = 1;
                else if (y<perlin && y>perlin-4)
                    blockID = 1; 
                else if (y<= perlin-4)
                    blockID = 1; 
                block_array.at(x, y, z).blockID = blockID;
            }
        }
    }

    Chunk chunk = Chunk(chunk_pos, block_array);
    return chunk;
}





// Chunk WorldGenerator::generateChunk(glm::ivec2 chunk_pos) 
// {   
//     // generates block_array then passes it to chunk constructor for lighting and everything else

//     //mountains
//     std::vector<float> sizes = {32};
//     std::vector<float> amplitudes = {100};

//     Array3D block_array;
//     //init chunk here
//     for (int x=0; x<CH_WIDTH; x++)
//     {
//         for (int z=0; z<CH_WIDTH; z++)
//         {
//             for (int y=0; y<CH_HEIGHT; y++)
//             {
//                 unsigned int blockID = 0;
//                 int perlin = (int) simplexOctaves3D(x+chunk_pos.x*CH_WIDTH, y, z+chunk_pos.y*CH_WIDTH, sizes, amplitudes);
//                 if(perlin>50)
//                 {
//                     blockID = 1;
//                 }
//                 block_array.at(x, y, z).blockID = blockID;
//             }
//         }
//     }

//     Chunk chunk = Chunk(chunk_pos, block_array);
//     return chunk;
// }




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


float WorldGenerator::simplexOctaves3D(int world_x, int world_y, int world_z, std::vector<float>& sizes, std::vector<float>& amplitudes)
{
    float noise = 0.0f;

    for (int i=0; i<sizes.size(); i++)
    {
        float amplitude = amplitudes[i];
        float size = sizes[i];
        noise += simplexNoise3D(world_x/size, world_y/size, world_z/size, amplitude);
    }
    return noise;
}



float WorldGenerator::simplexNoise2D(float x, float y, float amplitude) //"world" x and y, can be scaled
{
    float noise = 0.0f;
    int modulus = 4;

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
        noise += max*max*max*max * glm::dot(gradients_noise_2D[randomInt2D(vert, modulus)], glm::vec2(x_n, y_n));
    }

    //scale to [0, amplitude]
    return (70.0f * noise +1.0f)/2.0f * amplitude;
}



float WorldGenerator::simplexNoise3D(float x, float y, float z, float amplitude) //"world" x, y and z, can be scaled
{
    float noise = 0.0f;
    int modulus = 8;

    //scewed coordinates 
    float NNN = 3.0f;
    float F = (std::sqrt(NNN+1)-1)/NNN;
    float G = (1-1/std::sqrt(NNN+1))/NNN;
    float x_scew = x + (x+y+z)*F;
    float y_scew = y + (x+y+z)*F;
    float z_scew = z + (x+y+z)*F;

    //which hypercube coordinates 
    float x_b = std::floor(x_scew);
    float y_b = std::floor(y_scew);
    float z_b = std::floor(z_scew);

    //internal coordinates
    float x_i = x_scew-x_b;
    float y_i = y_scew-y_b;
    float z_i = z_scew-z_b;

    //order for edge traversal
    glm::ivec3 corner = glm::ivec3(x_b, y_b, z_b);
    glm::ivec3 last_corner = corner + glm::ivec3(1, 1, 1);
    std::vector<glm::ivec3> corners = {corner};

    //todo
    if(x_i > y_i && x_i > z_i)
    {
        corner = corner + glm::ivec3(1, 0, 0);
        corners.push_back(corner);
        if(y_i > z_i)
            corners.push_back(corner + glm::ivec3(0, 1, 0));
        else
            corners.push_back(corner + glm::ivec3(0, 0, 1));
    }
    if(y_i > x_i && y_i > z_i)
    {
        corner = corner + glm::ivec3(0, 1, 0);
        corners.push_back(corner);
        if(x_i > z_i)
            corners.push_back(corner + glm::ivec3(1, 0, 0));
        else
            corners.push_back(corner + glm::ivec3(0, 0, 1));
    }
    if(z_i > x_i && z_i > y_i)
    {
        corner = corner + glm::ivec3(0, 0, 1);
        corners.push_back(corner);
        if(x_i > y_i)
            corners.push_back(corner + glm::ivec3(1, 0, 0));
        else
            corners.push_back(corner + glm::ivec3(0, 1, 0));
    }
    corners.push_back(last_corner);

    //traverse every edge
    for (glm::ivec3& vert : corners)
    {   
        float factor = (vert.x+vert.y+vert.z)*G;
        float x_n = x-(vert.x - factor);
        float y_n = y-(vert.y - factor);
        float z_n = z-(vert.z - factor);

        float max = std::max(0.0f, 0.5f - x_n*x_n - y_n*y_n - z_n*z_n);
        noise += max*max*max*max * glm::dot(gradients_noise_3D[randomInt3D(vert, modulus)], glm::vec3(x_n, y_n, z_n));
    }

    //scale to [0, amplitude]
    return (32.0f * noise +1.0f)/2.0f * amplitude;
}


int WorldGenerator::randomInt2D(glm::ivec2 pos, int modulus) //TODO integrate seed
{
    // //std::lock_guard<std::mutex> lock(rand_mutex);
    // int r = ((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0));
    // return r % modulus;
    int N=3;
    glm::uvec2 v = (glm::uvec2) pos;
    uint s=0x9E3779B9u;
    for(int i=0; i<N; ++i) {
        v.x += ((v.y<<4u)+0xA341316Cu)^(v.y+s)^((v.y>>5u)+0xC8013EA4u);
        v.y += ((v.x<<4u)+0xAD90777Du)^(v.x+s)^((v.x>>5u)+0x7E95761Eu);
        s += 0x9E3779B9u;
    }
    return (v.x^v.y) % modulus;
}


int WorldGenerator::randomInt3D(glm::vec3 pos, int modulus) //TODO integrate seed
{
    //std::lock_guard<std::mutex> lock(rand_mutex);
    srand(((((((pos.x-0)*29 + 11)*13)+17 + 27*(pos.y-0))*23)*172 + (pos.z-0)*11));
    return rand() % modulus;
}