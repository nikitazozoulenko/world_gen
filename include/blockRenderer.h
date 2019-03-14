#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "shaderprogram.h"
#include "gameWorld.h"
#include "camera.h"

class BlockRenderer
{
public:
    Shaderprogram block_shaderprogram;

    BlockRenderer(GameWorld* p_game_world, Camera* p_camera);
    void render();
    void setup();
private:
    GameWorld* p_game_world;
    Camera* p_camera;
};
#endif // BLOCKRENDERER_H