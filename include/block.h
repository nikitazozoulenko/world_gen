#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>

#include "shaderprogram.h"

class BlockModel
{
public:
    BlockModel(std::string folder);
    void draw(Shaderprogram& shaderprogram, glm::vec3 world_pos);
    void setup(std::string folder);
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int diffuse_map;
    unsigned int specular_map;
};


class BlockInfo //TODO
{
    unsigned int blockID; //0 is air
    //lightning, other things needed to be block-specific
};


class Chunk
{
public:
    Chunk(glm::vec3 position);
    void draw(Shaderprogram& shaderprogram);
private:
    static const unsigned int WIDTH = 32;
    static const unsigned int HEIGHT = 32;
    glm::vec3 position;
    unsigned int blocks_array[WIDTH][HEIGHT][WIDTH];
    //std::unordered_map<glm::vec3, BlockInfo> block_info_map;   vec3 needs hash function  
    //std::unordered_map<unsigned int, BlockModel> block_model_map;
    BlockModel block_model = BlockModel("grass");
};

#endif // BLOCK_CHUNK_H