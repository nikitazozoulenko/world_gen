#include "../include/blockRenderer.h"

#include <iostream>

BlockRenderer::BlockRenderer(GameWorld* p_game_world, Camera* p_camera)
{
    this->p_game_world = p_game_world;
    this->p_camera = p_camera;
    createModelMap();
    createShaders();
}


void BlockRenderer::createModelMap()
{
    std::cout << "creating model map" << std::endl;
    BlockModel grass = BlockModel("grass");
    BlockModel metal = BlockModel("metal");
    BlockModel container = BlockModel("container");
    model_map[1] = grass;
    model_map[2] = metal; 
    model_map[3] = container;
}


void BlockRenderer::render()
{
    //bind shaders
    block_shaderprogram.use();

    //update view matrix every frame
    glm::mat4 view = p_camera->GetViewMatrix();
    block_shaderprogram.setUniformMat4("view", view);

    //render cubes
    for (Chunk& chunk : p_game_world->chunks)
    {
        chunk.draw(block_shaderprogram, model_map);
    }
}


void BlockRenderer::createShaders()
{
    block_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/block.vs", "/home/nikita/Code/world_gen/src/shaders/block.fs");

    // shader configuration
    block_shaderprogram.use(); //important to bind first
    block_shaderprogram.setUniformInt("material.diffuse", 0);
    block_shaderprogram.setUniformInt("material.specular", 1);
    block_shaderprogram.setUniformFloat("material.shininess", 32.0f);

    // directional light, (sun)
    block_shaderprogram.setUniformVec3("dirLight.direction", p_game_world->sun_direction);
    block_shaderprogram.setUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    block_shaderprogram.setUniformVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
    block_shaderprogram.setUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

}