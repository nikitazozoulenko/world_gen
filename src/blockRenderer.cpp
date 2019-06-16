#include "../include/blockRenderer.h"
#include "../include/misc.h"

BlockRenderer::BlockRenderer(Camera* p_camera, World* p_world) :
    p_camera(p_camera),
    p_world(p_world),
    block_model(BlockModel())
{
    createShaders();
    setProjectionMatrix();
}


void BlockRenderer::render()
{
    //bind shaders
    block_shaderprogram.use();

    //update view matrix every frame   (proj matrix really should NOT be set every frame)
    setProjectionMatrix();
    setViewMatrix();

    //render cubes
    for(auto& pair: p_world->getChunkMap())
    {
        pair.second.draw(block_shaderprogram, block_model);
    }
}


void BlockRenderer::setProjectionMatrix()
{
    glm::mat4 projection = glm::perspective(glm::radians(p_camera->zoom), (float)1280 / (float)720, 0.1f, 1000.0f);
    block_shaderprogram.setUniformMat4("projection", projection);
}


void BlockRenderer::setViewMatrix()
{
    glm::mat4 view = p_camera->getViewMatrix();
    block_shaderprogram.setUniformMat4("view", view);
}


void BlockRenderer::createShaders()
{
    block_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/block.vs", "/home/nikita/Code/world_gen/src/shaders/block.fs");

    // shader configuration UNIFORMS, removed atm
    //sunlight pos maybe for lighting
    block_shaderprogram.use();
}