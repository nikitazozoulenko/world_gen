#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <array>

#include "shaderprogram.h"

class BlockModel
{
public:
    BlockModel(); //this is required for unordered_map
    BlockModel(std::string folder);
    BlockModel(const BlockModel &obj);
    void draw(Shaderprogram& shaderprogram, glm::vec3 world_pos);
    void setup(std::string folder);
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int diffuse_map;
    unsigned int specular_map;
};


struct BlockInfo //TODO
{
public:
    unsigned int blockID = 0; //0 is air
    //lightning, other things needed to be block-specific
};


template <uint x, uint y, uint z>
class Array3D
{
public:
    Array3D() = default;
    BlockInfo& at(int a, int b, int c)
    {
        return data[a+x*b+x*y*c];
    }
private:
    std::array<BlockInfo, x*y*z> data;
};


class Chunk
{
public:
    static const unsigned int WIDTH = 32;
    static const unsigned int HEIGHT = 32;

    Chunk(glm::vec2 position, Array3D<WIDTH, HEIGHT, WIDTH> blocks_array);
    void draw(Shaderprogram& shaderprogram);
private:
    glm::vec2 position;
    Array3D<WIDTH, HEIGHT, WIDTH> block_array;
    std::unordered_map<unsigned int, BlockModel> block_model_map;
};

#endif // BLOCK_CHUNK_H