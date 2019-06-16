#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "blockRenderer.h"
#include "world.h"

class MasterRenderer
{
public:
    MasterRenderer(Camera* p_camera, World* p_world);
    void render();
private:
    BlockRenderer block_renderer;
    Camera* p_camera;
    World* p_world;
};
#endif // MASTERRENDERER_H