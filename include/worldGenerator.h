#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <functional>
#include <mutex>

#include "block.h"
#include "chunk.h"


class WorldGenerator
{
public:
    WorldGenerator(int seed=0);
    Chunk generateChunk(glm::vec2 position);
private:
    int seed;
    glm::vec2 gradients[4]; //2dim is 4 vectors
    float lerp(float val0, float val1, float x);
    float totalPerlinValue(int world_x, int world_z);
    float getPerlinValue(float x_norm, float y_norm, glm::vec2 corner_pos, float amplitude);
    int random(glm::vec2 pos);

    std::mutex rand_mutex;
};

#endif // WORLDGENERATOR_H