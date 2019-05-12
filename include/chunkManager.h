#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "block.h"
#include "chunk.h"
#include "worldGenerator.h"
#include "player.h"

#include <mutex>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

class ChunkManager
{
public:
    ChunkManager(); //dont use
    ChunkManager(Player& player);
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

    Player& player;
    int chunk_view_distance;
    bool stay_alive;
    std::mutex ch_map_mutex;
    std::mutex ch_being_generated_set_mutex;
    std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> ch_being_generated_set;
    std::unordered_set<glm::ivec2, std::hash<glm::ivec2>> ch_del_pos_set;
    std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> chunk_map;
    int n_workers;
    WorldGenerator world_gen;

    void updateLighting(glm::ivec2& pos);
    void updateLightingChunkEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2, std::unordered_set<glm::vec3, std::hash<glm::vec3>>& points);
    void globalLightBFSHelperFunc(int light_value, int x, int y, int z, std::unordered_set<glm::vec3, std::hash<glm::vec3>>& new_points);
    void globalRecursiveLightBFS(std::unordered_set<glm::vec3, std::hash<glm::vec3>>& points);
    void updateVisible(int x, int y, int z);
    void updateVisible(float x, float y, float z);
    void addChunk(glm::ivec2 chunk_pos);
    void updateBlockVisEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2);
    void updateEdges(glm::ivec2& pos);
    void removeChunksOutOfRange();
    void blockChangePropagateDownSunlight(int x, int yy, int z, std::unordered_set<glm::vec3, std::hash<glm::vec3>>& points);
private:

};

#endif // CHUNKMANAGER_H
