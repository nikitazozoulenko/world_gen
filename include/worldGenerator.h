#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "gameWorld.h"
#include "block.h"

class WorldGenerator 
{
public:
    WorldGenerator(int seed = 0);
    Chunk generateChunk(glm::vec2 position);
private:
    GameWorld* p_game_world;
    glm::vec2 gradients[4]; //2d is 4
    void setSeed(int seed);
    float lerp(float val0, float val1, float x);
    float getPerlinValue(float x_norm, float y_norm, glm::vec2 world_pos);
    int random(glm::vec2 pos);
};

#endif // WORLDGENERATOR_H
