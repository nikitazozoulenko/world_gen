#include "../include/objectRenderer.h"

#include <iostream>

ObjectRenderer::ObjectRenderer(GameWorld* p_game_world)
{
    this->p_game_world = p_game_world;
    createShaders();
}


void ObjectRenderer::render()
{
    //bind shaders
    obj_shaderprogram.use();

    //update view matrix every frame   (proj matrix really should NOT be set every frame)
    setProjectionMatrix();
    setViewMatrix();

    //draw our model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    obj_shaderprogram.setUniformMat4("model", model);
    ourModel.draw(obj_shaderprogram);
}


void ObjectRenderer::setProjectionMatrix()
{
    glm::mat4 projection = glm::perspective(glm::radians(p_game_world->player.camera.zoom), (float)1600 / (float)900, 0.1f, 1000.0f);
    obj_shaderprogram.setUniformMat4("projection", projection);
}


void ObjectRenderer::setViewMatrix()
{
    glm::mat4 view = p_game_world->player.camera.getViewMatrix();
    obj_shaderprogram.setUniformMat4("view", view);
}


void ObjectRenderer::createShaders()
{
    obj_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/object.vs", "/home/nikita/Code/world_gen/src/shaders/object.fs");

    //TODO set lighting and such here:
}