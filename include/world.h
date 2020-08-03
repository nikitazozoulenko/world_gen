#ifndef MY_WORLD_H
#define MY_WORLD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>


#include <unordered_map>
#include <string>
#include <atomic>
#include <unordered_set>
#include <queue>

#include <chunk.h>
#include <settings.h>
#include <shaderprogram.h>
#include <camera.h>
#include <player.h>


typedef std::unordered_map<glm::ivec2, Chunk, std::hash<glm::ivec2>> ChunkMapivec2; 

struct ChunkGenInfo //multiple threads have access to this
{
    int stage;
    float* height_map;
    float* rng_map;
    std::vector<glm::vec2> tree_points;
    unsigned int* blocks;
    int num_render_faces[6] = {0,0,0,0,0,0};
    std::unordered_map<glm::vec3, unsigned int, std::hash<glm::vec3>> render_faces_map[6];
    bool added_to_queue = false;
};


class ThreadPool{
public:
    ThreadPool(int n_workers);

    boost::asio::io_service ioService;
    boost::thread_group thread_group;
    boost::asio::io_service::work work;
    int n_workers;

    void stop();
private:
};


class ChunkManager
{
public:
    ChunkManager(Settings& settings, ChunkMapivec2& chunk_map, std::unordered_map<std::string, unsigned int>& blockIDMap);
    std::vector<glm::ivec2> circle_chunk_positions;



    void sort_circle();

    unsigned int& getBlock(glm::vec3 v);
    unsigned int& getBlock(int x, int y, int z);
    bool isInBounds(glm::vec3 v);
    bool isInBounds(int x, int y, int z);
    void setBlock(glm::vec3 v, int blockID);
    void setBlock(int x, int y, int z, int blockID);
    std::pair<glm::ivec2, glm::vec3> findChunkPos(int x, int y, int z);
    std::pair<glm::ivec2, glm::vec3> findChunkPos(glm::vec3 v);
    bool isInChunk(glm::ivec2 ch_pos, int x, int y, int z);

    void updateVisible(int x, int y, int z);

    std::vector<glm::vec2> octaves; //(size, amp)
    std::vector<float> sizes;
    std::vector<float> amplitudes;
    std::unordered_map<std::string, unsigned int>& blockIDMap;


    unsigned int determineBlock(int x, int y, int z, int water_level, float low_snow_level, float high_snow_level, float* height, float* prng);
    void gen_nearby_chunks(glm::vec3& center_pos);
    std::unordered_map<glm::ivec2, ChunkGenInfo, std::hash<glm::ivec2>> gen_info_map;
    void stageOneGen(glm::ivec2& pos);
    void setInfoGenBlock(int x, int y, int z, unsigned int blockID);
    unsigned int getInfoGenBlock(int x, int y, int z);
    void placeTree(glm::ivec2& pos, int x, int y);
    void stageTwoDecoration(glm::ivec2& pos);
    void addToRenderMap(ChunkGenInfo& info, int blockID, int face, glm::vec3 pos);
    void stageThreeChunkCreation(glm::ivec2& pos);
    const int n_threads;
    ThreadPool pool;
    std::atomic<int> work_queue_size;
    std::unordered_map<glm::ivec2, std::atomic<bool>, std::hash<glm::ivec2>> busy;
    boost::shared_mutex shared_mutex;
    void visibilityInner(ChunkGenInfo& info, glm::ivec2& pos);
    void visibilityInnerAtPos(ChunkGenInfo& info, glm::ivec2& pos, int face, int x, int y, int z, unsigned int blockID);
    std::queue<glm::ivec2> finished_positions;
    void add_new_chunks();
    void visibilityOuter(ChunkGenInfo& info, glm::ivec2& pos, ChunkGenInfo& info2, int face);
    void remove_far_chunks(glm::vec3& camera_pos);

private:
    Settings& settings;
    ChunkMapivec2& chunk_map;
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



