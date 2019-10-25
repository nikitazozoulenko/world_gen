#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

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
    Shaderprogram march_cube_draw_shaderprogram;
    unsigned int comp_texture;

    void createShaders();
};
#endif // BLOCKRENDERER_H