#ifndef BLOCK_BLOCK_H
#define BLOCK_BLOCK_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <array>
#include <vector>

#include "shaderprogram.h"

class BlockModel
{
public:
    BlockModel();
    enum{
        EAST,
        WEST,
        TOP,
        BOTTOM,
        NORTH,
        SOUTH,
    };
    std::array<unsigned int, 6> VAOs; 
    std::array<unsigned int, 6> VBOs;
    std::array<unsigned int, 6> mat_VBOs;
    std::array<unsigned int, 6> blockID_VBOs;
    std::array<unsigned int, 6> light_VBOs;
private:
    void setup_VAO_VBO();
    void setup_face(std::vector<float> vertices, int side);
};



struct BlockInfo
{
public:
    int blockID = 0; //0 is air
    float lighting = 1.0f; //0 to 15
};

#endif // BLOCK_BLOCK_H