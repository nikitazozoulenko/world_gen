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

#include <array>
#include <vector>

#define CH_WIDTH 16
#define CH_HEIGHT 128

#include "block.h"

struct RenderBlockInfo
{
    int blockID;
    float face_lighting; //TODO change from float
};



class Array3D
{
public:
    Array3D() {}
    inline BlockInfo& at(int x, int y, int z) { return data[x+CH_WIDTH*y+CH_WIDTH*CH_HEIGHT*z]; }
private:
    std::array<BlockInfo, CH_WIDTH*CH_WIDTH*CH_HEIGHT> data;
};


class Chunk
{
public:
    glm::vec2 position;

    bool re_init_vaos = true;
    bool first_vbo_init = true;

    void addToRenderMap(int blockID, int face, float lighting, glm::vec3 pos);
    void removeFromRenderMap(int face, glm::vec3 pos);

    Chunk() = default;  //for std map
    Chunk(glm::vec2 position, const Array3D& blocks_array);
    BlockInfo& getBlockInfo(int x, int y, int z);

private:
    Array3D block_array;
    int num_render_faces[6] = {0,0,0,0,0,0};
    std::unordered_map<glm::vec3, RenderBlockInfo, std::hash<glm::vec3>> render_faces_map[6];

    void visibiltyChecking();
    bool blockIsInChunk(int local_x, int local_y, int local_z);
    void visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID);
    void rebuildVBOs(std::array<std::unordered_map<int, int>,6>& );
};

#endif // BLOCK_CHUNK_H