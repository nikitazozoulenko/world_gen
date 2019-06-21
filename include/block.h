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
    ~BlockModel();

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
    std::array<unsigned int, 6> pos_VBOs;
    std::array<unsigned int, 6> block_VBOs;

private:
    void setup_VAO_VBO();
    void setup_face(std::vector<float> vertices, int side);
};

#endif // BLOCK_BLOCK_H