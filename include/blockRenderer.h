#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"
#include "gameWorld.h"
#include "block.h"

class BlockRenderer
{
public:
    Shaderprogram block_shaderprogram;

    BlockRenderer(GameWorld* p_game_world);
    void render();
    void setProjectionMatrix();
    void setViewMatrix();
    
private:
    GameWorld* p_game_world;

    unsigned int block_texture;

    void createModelMap();
    void createShaders();
    void setup_block_texture();
};
#endif // BLOCKRENDERER_H