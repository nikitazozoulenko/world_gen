#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "blockRenderer.h"

class MasterRenderer
{
public:
    MasterRenderer(Camera* p_camera);
    void render();
private:
    BlockRenderer block_renderer;
    Camera* p_camera;
};
#endif // MASTERRENDERER_H