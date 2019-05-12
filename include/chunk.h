#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <unordered_map>
#include <string>
#include <array>
#include <unordered_set>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "shaderprogram.h"
#include "block.h"

#define CH_WIDTH 16
#define CH_HEIGHT 64


class Array3D
{
public:
    Array3D() {}
    inline BlockInfo& at(int x, int y, int z) { return data[x+CH_WIDTH*y+CH_WIDTH*CH_HEIGHT*z]; }
private:
    std::array<BlockInfo, CH_WIDTH*CH_WIDTH*CH_HEIGHT> data;
};


struct RenderBlockInfo
{
    int blockID;
    float face_lighting; //TODO change from float
};


//every block is stored in an array3D of BlockInfos
//for rendering we use a map of vectors of glm::vec3s (index to array3D)
class Chunk
{
public:
    glm::vec2 position;
    bool re_init_vaos;
    bool first_vbo_init;


    void addToRenderMap(int blockID, int face, int lighting, glm::vec3 pos);
    void removeFromRenderMap(int face, glm::vec3 pos);

    Chunk() = default;  //for std map
    Chunk(glm::vec2 position, const Array3D& blocks_array);
    ~Chunk();

    void draw(Shaderprogram& shaderprogram, glm::vec3& view_dir, std::array<std::unordered_map<int, int>,6>& texArrayIDLookup);
    BlockInfo& getBlockInfo(int x, int y, int z);

private:
    Array3D block_array;
    BlockModel* p_block_model;
    int num_render_faces[6] = {0,0,0,0,0,0};
    std::unordered_map<glm::vec3, RenderBlockInfo, std::hash<glm::vec3>> render_faces_map[6];
    int sunlight_level[CH_WIDTH][CH_WIDTH];

    void visibiltyChecking();
    bool helperFunInterestingAdjacantPoint(int x, int y, int z);
    void lightBFSHelperFunc(int light_value, int x, int y, int z, std::unordered_set<glm::vec3, std::hash<glm::vec3>>& new_points);
    void recursiveLightBFS(std::unordered_set<glm::vec3, std::hash<glm::vec3>>& points);
    void sunlightChecking();
    bool blockIsInChunk(int local_x, int local_y, int local_z);
    void visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID);
    void rebuildVBOs(std::array<std::unordered_map<int, int>,6>& );
};

#endif // BLOCK_CHUNK_H