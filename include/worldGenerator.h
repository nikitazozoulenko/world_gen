#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <functional>

#include "block.h"

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
    float getPerlinValue(float x_norm, float y_norm, glm::vec2 corner_pos, int amplitude);
    int random(glm::vec2 pos);
};

#endif // WORLDGENERATOR_H
