#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>
#include <chunk.h>

class BlockRenderer
{
public:
    BlockRenderer(Settings* p_settings);
    void render(Camera* p_camera);
    
private:
    Shaderprogram march_cube_draw_shaderprogram;
    Shaderprogram comp_shaderprogram;

    Settings* p_settings;
    unsigned int comp_texture;
    

    void createShaders();
    void createMarchComputeTexture();
    Chunk createChunk(glm::ivec2 pos);
};
#endif // BLOCKRENDERER_H