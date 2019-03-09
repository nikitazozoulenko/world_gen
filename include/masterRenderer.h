#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "blockRenderer.h"
#include "camera.h"
#include "model.h"

class MasterRenderer
{
public:
    MasterRenderer(GameWorld* p_game_world, Camera* p_camera);
    void render();
private:
    BlockRenderer block_renderer;
    Camera* p_camera;

    //temporary
    Model ourModel = Model("/home/nikita/Code/world_gen/resources/nanosuit/nanosuit.obj");
    Shaderprogram obj_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/object.vs", "/home/nikita/Code/world_gen/src/shaders/object.fs");

};
#endif // MASTERRENDERER_H