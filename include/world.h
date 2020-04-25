#ifndef MY_WORLD_H
#define MY_WORLD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>

#include <chunk.h>
#include <settings.h>
#include <shaderprogram.h>
#include <camera.h>

typedef std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> ChunkMapivec2; 


class ChunkManager
{
public:
    ChunkManager(Settings& settings, ChunkMapivec2& chunk_map);
    void createChunk(glm::ivec2 pos);

    float size;
    float amplitude;
private:
    Settings& settings;
    ChunkMapivec2& chunk_map;
    Shaderprogram comp_shaderprogram;
    unsigned int comp_texture;

    void createComputeShader();
    void createMarchComputeTexture();
};


class World
{
public:
    World(Settings& settings);
    ChunkMapivec2 chunk_map;
    ChunkManager chunk_manager;
private:
    Settings& settings;
};




#endif // MY_WORLD_H



