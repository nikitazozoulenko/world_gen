#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <unordered_map>
#include <string>
#include <array>
#include <unordered_set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


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
    static const int WIDTH = 32;    // x
    static const int HEIGHT = 100;  // y
    static const int BREADTH = 32;  // z

    Chunk() = default;  //for std map
    Chunk(glm::vec2 position, Array3D<WIDTH, HEIGHT, BREADTH> blocks_array);
    void draw(Shaderprogram& shaderprogram, std::unordered_map<unsigned int, BlockModel>& model_map);
    BlockInfo& getBlockInfo(int x, int y, int z);
    std::unordered_map<unsigned int, std::unordered_set<glm::vec3, std::hash<glm::vec3>>> block_locations;
private:
    glm::vec2 position;
    Array3D<WIDTH, HEIGHT, BREADTH> block_array;
};

#endif // BLOCK_CHUNK_H