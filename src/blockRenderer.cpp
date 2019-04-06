#include "../include/blockRenderer.h"

#include <iostream>
#include "../include/camera.h"
#include "../include/misc.h"

BlockRenderer::BlockRenderer(GameWorld* p_game_world)
{
    this->p_game_world = p_game_world;

    createShaders();
    setProjectionMatrix();
    setup_block_texture();
}


void BlockRenderer::render()
{
    //bind shaders
    block_shaderprogram.use();

    //update view matrix every frame   (proj matrix really should NOT be set every frame)
    setProjectionMatrix();
    setViewMatrix();

    //render cubes
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture);
    for (auto& pair : p_game_world->chunks)
    {
        pair.second.draw(block_shaderprogram, p_game_world->player.camera.front);
    }
}


void BlockRenderer::setProjectionMatrix()
{
    glm::mat4 projection = glm::perspective(glm::radians(p_game_world->player.camera.zoom), (float)1600 / (float)900, 0.1f, 1000.0f);
    block_shaderprogram.setUniformMat4("projection", projection);
}


void BlockRenderer::setViewMatrix()
{
    glm::mat4 view = p_game_world->player.camera.getViewMatrix();
    block_shaderprogram.setUniformMat4("view", view);
}


void BlockRenderer::createShaders()
{
    block_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/block.vs", "/home/nikita/Code/world_gen/src/shaders/block.fs");

    // shader configuration
    //removed atm
    //sunlight pos maybe for lighting
    block_shaderprogram.use();
    block_shaderprogram.setUniformVec3("sun_dir", p_game_world->sun_direction);
}


void BlockRenderer::setup_block_texture()
{
    int block_width = 512;
    int n_blocks = 8;
    std::string path = "/home/nikita/Code/world_gen/resources/blocks/wholeblocktextures.png";
    block_texture = loadTextureArray(path.c_str(), block_width, n_blocks);
}