#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"
#include "gameWorld.h"
#include "camera.h"
#include "model.h"

class ObjectRenderer
{
public:
    Shaderprogram obj_shaderprogram;

    ObjectRenderer(GameWorld* p_game_world);
    void render();
    void setProjectionMatrix();
    void setViewMatrix();

private:
    GameWorld* p_game_world;

    //this should be in p_game_world (or in a Player class?)
    Model ourModel = Model("/home/nikita/Code/world_gen/resources/nanosuit/nanosuit.obj");

    void createShaders();
};
#endif // OBJECTRENDERER_H