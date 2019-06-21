#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <unordered_map>
#include <string>
#include <array>
#include <unordered_set>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "shaderprogram.h"
#include "block.h"

#define CH_WIDTH 10
#define CH_HEIGHT 100


class Array3D
{
public:
    Array3D() {}
    inline int& at(int x, int y, int z) { return data[x+CH_WIDTH*y+CH_WIDTH*CH_HEIGHT*z]; }
private:
    std::array<int, CH_WIDTH*CH_WIDTH*CH_HEIGHT> data;
};


class Chunk
{
public:
    Chunk() = default;  //for std map
    Chunk(glm::ivec2 position);

    void draw(Shaderprogram& shaderprogram, BlockModel& block_model);
    int buildVBOs(BlockModel& block_model);
    int& getBlock(int x, int y, int z);
private:
    glm::ivec2 position;
    Array3D block_array;
};

#endif // BLOCK_CHUNK_H