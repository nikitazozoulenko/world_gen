#include "../include/masterRenderer.h"

#include <iostream>

MasterRenderer::MasterRenderer(GameWorld* p_game_world, Camera* p_camera) : block_renderer(BlockRenderer(p_game_world, p_camera)), p_camera(p_camera)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    //update projection matrix ONLY ONCE
    glm::mat4 projection = glm::perspective(glm::radians(p_camera->zoom), (float)1600 / (float)900, 0.1f, 1000.0f);
    block_renderer.block_shaderprogram.use();
    block_renderer.block_shaderprogram.setUniformMat4("projection", projection);
    obj_shaderprogram.use();
    obj_shaderprogram.setUniformMat4("projection", projection);
}


void MasterRenderer::render()
{
    //once every frame
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //blocks
    block_renderer.render();

    //objects
    obj_shaderprogram.use();
    glm::mat4 view = p_camera->GetViewMatrix();
    obj_shaderprogram.setUniformMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(p_camera->zoom), (float)960 / (float)540, 0.1f, 1000.0f);
    obj_shaderprogram.use();
    obj_shaderprogram.setUniformMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
    obj_shaderprogram.setUniformMat4("model", model);
    ourModel.draw(obj_shaderprogram);
}