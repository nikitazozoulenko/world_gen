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
    glm::vec2 gradients_noise_2D[4]; //2dim is 4 vectors
    glm::vec3 gradients_noise_3D[8]; //2dim is 4 vectors

    int randomInt2D(glm::vec2 pos, int modulus);
    int randomInt3D(glm::vec3 pos, int modulus);
    float simplexNoise2D(float x, float z, float amplitude);
    float simplexOctaves2D(int x, int z, std::vector<float>& sizes, std::vector<float>& amplitudes);
    float simplexNoise3D(float x, float y, float z, float amplitude);
    float simplexOctaves3D(int x, int y, int z, std::vector<float>& sizes, std::vector<float>& amplitudes);

    std::mutex rand_mutex;
};

#endif // WORLDGENERATOR_H