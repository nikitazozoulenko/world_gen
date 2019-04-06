#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <functional>

#include "block.h"
#include "chunk.h"

class WorldGenerator
{
public:
    WorldGenerator(int seed=0);
    Chunk generateChunk(glm::vec2 position);
    std::unordered_map<unsigned int, BlockModel> block_model_map;
private:
    int seed;
    glm::vec2 gradients[4]; //2dim is 4 vectors
    float lerp(float val0, float val1, float x);
    float totalPerlinValue(int world_x, int world_z);
    float getPerlinValue(float x_norm, float y_norm, glm::vec2 corner_pos, float amplitude);
    void visibiltyChecking(Array3D<2+Chunk::WIDTH, Chunk::HEIGHT, 2+Chunk::BREADTH>& block_array, Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::BREADTH>& final_block_array);
    void addSunlight(Array3D<2+Chunk::WIDTH, Chunk::HEIGHT, 2+Chunk::BREADTH>& block_array, Array3D<Chunk::WIDTH, Chunk::HEIGHT, Chunk::BREADTH>& final_block_array);
    int random(glm::vec2 pos);
};

#endif // WORLDGENERATOR_H
