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

    int random(glm::vec2 pos);
    float simplexNoise2D(float x, float y, float amplitude);
    float simplexOctaves2D(int world_x, int world_z, std::vector<float>& sizes, std::vector<float>& amplitudes);


    std::mutex rand_mutex;
};

#endif // WORLDGENERATOR_H