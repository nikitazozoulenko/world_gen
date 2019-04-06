#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "block.h"
#include "worldGenerator.h"
#include "player.h"
#include "chunk.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class GameWorld
{
public:
    Player player;
    glm::vec3 sun_direction;
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> chunks;

    bool block_is_targeted;
    glm::vec3 target_pos;
    glm::vec3 before_pos;

    GameWorld(WorldGenerator* p_world_gen);
    void changeBlock(int x, int y, int z, int blockID);
    void changeBlock(float x, float y, float z, int blockID);
    BlockInfo& getBlockInfo(int x, int y, int z);
    BlockInfo& getBlockInfo(float x, float y, float z);
    bool isInBounds(int x, int y, int z);
    bool isInBounds(float x, float y, float z);
    void updateVisible(int x, int y, int z, int offset);
    void updateVisible(float x, float y, float z, int offset);
    glm::mat4 getViewMatrix();
    void updateTarget();
    void targetBlockRay(float x, float y, float z, glm::vec3 previous_block_pos);

    void addChunk(glm::ivec2 chunk_pos, Chunk chunk);
private:
    WorldGenerator* p_world_gen;
};

#endif // GAMEWORLD_H