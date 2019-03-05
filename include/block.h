#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <glm/glm.hpp>

#include <unordered_map>

#include "shaderprogram.h"

class BlockModel
{
public:
    BlockModel();
    void draw(Shaderprogram& shaderprogram, glm::vec3 world_pos);
    void setup();
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int diffuse_map;
    unsigned int specular_map;
};


class BlockInfo
{
    unsigned int blockID;
    //lightning, other things needed to be block-specific
};


class Chunk
{
public:
    Chunk(glm::vec3 position);
    void draw(Shaderprogram& shaderprogram);
private:
    glm::vec3 position;
    //std::unordered_map<glm::vec3, BlockInfo> block_info_map;   vec3 needs hash function  
    std::unordered_map<unsigned int, BlockModel>* p_block_model_map; //this should maybe just be in the block renderer instead
};

#endif // BLOCK_CHUNK_H