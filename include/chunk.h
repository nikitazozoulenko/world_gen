#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>
#include <block.h>

#include <unordered_map>
#include <vector>

class Chunk
{
public:
    Chunk(Settings& settings, glm::ivec2 pos, unsigned int* blocks, int num_render_faces[6], 
        std::unordered_map<glm::vec3, unsigned int, std::hash<glm::vec3>> render_faces_map[6]);
    ~Chunk();   //destructor
    // Chunk(const Chunk& source); //copy
    // Chunk& operator=(const Chunk& source); //assign operator
    // Chunk& operator=(Chunk&& source); // move assignment

    void render(std::array<std::unordered_map<int, int>,6>& texArrayIDLookup);
    unsigned int& getBlock(int x, int y, int z);
    unsigned int& getBlock(glm::ivec3 v);
    unsigned int& getBlock(glm::vec3 v);
    void setBlock(int x, int y, int z, unsigned int block);
    void setBlock(glm::ivec3 v, unsigned int block);
    void setBlock(glm::vec3 v, unsigned int block);

    void addToRenderMap(int blockID, int face, glm::vec3 pos);
    void removeFromRenderMap(int face, glm::vec3 pos);

    void visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID);
    
    glm::ivec2 pos;
    bool re_init_vaos;
private:
    Settings& settings;

    BlockModel* p_block_model;

    unsigned int* blocks;
    float* height_map;
    int num_render_faces[6] = {0,0,0,0,0,0};
    std::unordered_map<glm::vec3, unsigned int, std::hash<glm::vec3>> render_faces_map[6];
    bool blockIsInChunk(int local_x, int local_y, int local_z);
    void rebuildVBOs(std::array<std::unordered_map<int, int>,6>& texArrayIDLookup);

};
#endif // WORLD_CHUNK_H