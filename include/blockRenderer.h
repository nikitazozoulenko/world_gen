#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "shaderprogram.h"
#include "world.h"
#include "camera.h"
#include "block.h"

class BlockRenderer
{
public:
    BlockRenderer(Camera* p_camera, World* p_world);
    void render();
    
private:
    Shaderprogram block_shaderprogram;
    BlockModel block_model;
    Camera* p_camera;
    World* p_world;

    void setProjectionMatrix();
    void setViewMatrix();
    void createShaders();
};
#endif // BLOCKRENDERER_H