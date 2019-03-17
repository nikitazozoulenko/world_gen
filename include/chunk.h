#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <array>
#include <vector>

#include "shaderprogram.h"
#include "block.h"

template <uint x, uint y, uint z>
class Array3D
{
public:
    Array3D() = default;
    inline BlockInfo& at(int a, int b, int c){ return data[a+x*b+x*y*c]; }
private:
    std::array<BlockInfo, x*y*z> data;
};



//every block is stored in an array3D of BlockInfos
//for rendering we use a map of vectors of glm::vec3s (index to array3D)
class Chunk
{
public:
    static const unsigned int WIDTH = 32;   // x
    static const unsigned int HEIGHT = 100;  // y
    static const unsigned int BREADTH = 32; // z

    Chunk(glm::vec2 position, Array3D<WIDTH, HEIGHT, BREADTH> blocks_array);
    void draw(Shaderprogram& shaderprogram, std::unordered_map<unsigned int, BlockModel>& model_map);
    void setBlock(int x, int y, int z, BlockInfo block_info);
    BlockInfo getBlock(int x, int y, int z);
    void removeBlock(int x, int y, int z);
private:
    glm::vec2 position;
    Array3D<WIDTH, HEIGHT, BREADTH> block_array;
    std::unordered_map<unsigned int, std::vector<glm::vec3>> block_locations;
};

#endif // BLOCK_CHUNK_H