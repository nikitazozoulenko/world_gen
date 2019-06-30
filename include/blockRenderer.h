#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "shaderprogram.h"
#include "camera.h"

class BlockRenderer
{
public:
    BlockRenderer(Camera* p_camera);
    void render();
    
private:
    Shaderprogram quad_shaderprogram;
    Shaderprogram ray_shaderprogram;
    Camera* p_camera;
    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int ray_texture;
    unsigned int block_texture;
    unsigned int block_normal;

    void setupQuad();
    void createShaders();
    void setupTextures();
};
#endif // BLOCKRENDERER_H