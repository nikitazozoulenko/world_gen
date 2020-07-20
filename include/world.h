#ifndef MY_WORLD_H
#define MY_WORLD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <string>

#include <chunk.h>
#include <settings.h>
#include <shaderprogram.h>
#include <camera.h>
#include <player.h>

typedef std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> ChunkMapivec2; 

class ChunkManager
{
public:
    ChunkManager(Settings& settings, ChunkMapivec2& chunk_map, std::unordered_map<std::string, unsigned int>& blockIDMap);
    void createChunk(glm::ivec2 pos);
    void removeChunk(glm::ivec2 pos);

    void gen_new_nearby_chunks(glm::vec3& center_pos);
    void remove_far_chunks(glm::vec3& center_pos);

    void updateEdges(glm::ivec2& pos);
    void updateBlockVisEdge(Chunk& chunk1, int face1, Chunk& chunk2, int face2);

    unsigned int& getBlock(glm::vec3 v);
    unsigned int& getBlock(int x, int y, int z);
    bool isInBounds(glm::vec3 v);
    bool isInBounds(int x, int y, int z);
    void setBlock(glm::vec3 v, int blockID);
    void setBlock(int x, int y, int z, int blockID);
    std::pair<glm::ivec2, glm::vec3> findChunkPos(int x, int y, int z);
    std::pair<glm::ivec2, glm::vec3> findChunkPos(glm::vec3 v);

    void updateVisible(int x, int y, int z);

    float size;
    float amplitude;
    std::unordered_map<std::string, unsigned int>& blockIDMap;
private:
    Settings& settings;
    ChunkMapivec2& chunk_map;
    Shaderprogram comp_shaderprogram;
    unsigned int comp_texture;

    void createComputeShader();
    void createComputeTexture();

    
};


class World
{
public:
    World(Settings& settings, Camera& camera, std::unordered_map<std::string, unsigned int>& blockIDMap);
    ChunkMapivec2 chunk_map;
    ChunkManager chunk_manager;
    Player player;
    std::unordered_map<std::string, unsigned int>& blockIDMap;

    bool block_is_targeted;
    glm::vec3 target_pos;
    glm::vec3 target_facing;
    void targetBlockRay(double x, double y, double z, glm::vec3 enter_direction = glm::vec3(0,0,0), int step=0);
    void targetBlockRay(glm::vec3 v, glm::vec3 enter_direction = glm::vec3(0,0,0), int step=0);
    unsigned int& getBlock(glm::vec3 v);
    unsigned int& getBlock(int x, int y, int z);
    bool isInBounds(glm::vec3 v);
    bool isInBounds(int x, int y, int z);
    void setBlock(glm::vec3 v, int blockID);
    void setBlock(int x, int y, int z, int blockID);

    void placeBlockOnCursor();
    void destroyBlockOnCursor();

    void gameLogic(double delta_time);
private:
    Settings& settings;
};




#endif // MY_WORLD_H



