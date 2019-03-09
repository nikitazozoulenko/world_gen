#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "gameWorld.h"

class WorldGenerator 
{
public:
    WorldGenerator(int seed = 0);
    void generateChunk(glm::vec2 position);
private:
    GameWorld* p_game_world;
    glm::vec2 gradients[4]; //2d is 4
    void setSeed(int seed);
    float lerp(float val0, float val1, float x);
    float getPerlinValue(int x, int y);
};

#endif // WORLDGENERATOR_H
