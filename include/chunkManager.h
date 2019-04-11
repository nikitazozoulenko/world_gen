#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "block.h"
#include "chunk.h"
#include "worldGenerator.h"

#include <mutex>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class ChunkManager
{
public:
    ChunkManager();
    void startMainThread();
    void endMainThread();
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>>& getChunkMap();
    std::mutex& getMutex();
    void changeBlock(int x, int y, int z, int blockID);
    void changeBlock(float x, float y, float z, int blockID);
    BlockInfo& getBlockInfo(int x, int y, int z);
    BlockInfo& getBlockInfo(float x, float y, float z);
    bool isInBounds(int x, int y, int z);
    bool isInBounds(float x, float y, float z);

private:
    WorldGenerator world_gen;
    bool stay_alive;
    std::mutex ch_map_mutex;
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> chunk_map;

    void updateVisible(int x, int y, int z, int offset);
    void updateVisible(float x, float y, float z, int offset);
    void addChunk(glm::ivec2 chunk_pos, Chunk chunk);
    void updateVisChunkEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2);
};

#endif // CHUNKMANAGER_H
