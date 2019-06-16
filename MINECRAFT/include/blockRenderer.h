#ifndef BLOCKRENDERER_H
#define BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

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
    std::array<std::unordered_map<int, int>,6> texArrayIDLookup;

    void createModelMap();
    void createShaders();
    void setup_block_texture();
    void setupTexArrayIDLookup(int n_blocks);
};
#endif // BLOCKRENDERER_H