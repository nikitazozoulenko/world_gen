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
    BlockModel(); //this is required for unordered_map
    BlockModel(std::string folder);
    BlockModel(const BlockModel &obj);
    void setup_side_cube(std::string folder);
    void setup_normal_cube(std::string folder);
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    int num_indices;

    unsigned int diffuse_map;
    unsigned int specular_map;
private:
};



struct BlockInfo
{
public:
    unsigned int blockID = 0; //0 is air
    bool visible = false;
    //lightning, other things needed to be block-specific
};

#endif // BLOCK_BLOCK_H