#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "blockRenderer.h"
#include "objectRenderer.h"
#include "uiRenderer.h"
#include "camera.h"

class MasterRenderer
{
public:
    MasterRenderer(GameWorld* p_game_world);
    void render();
private:
    BlockRenderer block_renderer;
    ObjectRenderer obj_renderer;
    UIRenderer ui_renderer;
    GameWorld* p_game_world;
};
#endif // MASTERRENDERER_H