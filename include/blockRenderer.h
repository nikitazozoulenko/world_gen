#ifndef MY_BLOCKRENDERER_H
#define MY_BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>
#include <chunk.h>
#include <world.h>

class BlockRenderer
{
public:
    BlockRenderer(Settings& settings);
    void render(ChunkMapivec2& chunk_map, Camera& camera);
    
private:
    Settings& settings;
    Shaderprogram block_shaderprogram;
    unsigned int block_texture;
    std::array<std::unordered_map<int, int>,6> texArrayIDLookup;

    void createModelMap();
    void createShaders();
    void setup_block_texture();
    void setupTexArrayIDLookup(int n_blocks);

    void setProjectionMatrix(Camera& camera);
    void setViewMatrix(Camera& camera);
};
#endif // MY_BLOCKRENDERER_H