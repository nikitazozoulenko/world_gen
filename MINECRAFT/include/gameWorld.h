#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "block.h"
#include "worldGenerator.h"
#include "player.h"
#include "chunk.h"
#include "chunkManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class GameWorld
{
public:
    Player player;
    glm::vec3 sun_direction;

    bool block_is_targeted;
    glm::vec3 target_pos;
    glm::vec3 before_pos;

    GameWorld();
    void changeBlock(int x, int y, int z, int blockID);
    void changeBlock(float x, float y, float z, int blockID);
    BlockInfo& getBlockInfo(int x, int y, int z);
    BlockInfo& getBlockInfo(float x, float y, float z);
    bool isInBounds(int x, int y, int z);
    bool isInBounds(float x, float y, float z);
    glm::mat4 getViewMatrix();
    void update();
    void targetBlockRay(float x, float y, float z, glm::vec3 previous_block_pos);

    ChunkManager chunk_manager;
private:
    int seed;  //TODO do things with this
};

#endif // GAMEWORLD_H