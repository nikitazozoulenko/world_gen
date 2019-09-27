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
    Shaderprogram comp_shaderprogram;
    Camera* p_camera;
    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int comp_texture;
    unsigned int image_width;
    unsigned int image_height;

    void setupQuad();
    void createShaders();
};
#endif // BLOCKRENDERER_H