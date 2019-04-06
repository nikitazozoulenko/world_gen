#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <unordered_map>
#include <string>
#include <array>
#include <unordered_set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


#include "shaderprogram.h"
#include "block.h"

template <int x, int y, int z>
class Array3D
{
public:
    Array3D() = default;
    inline BlockInfo& at(int a, int b, int c){ return data[a+x*b+x*y*c]; }
private:
    std::array<BlockInfo, x*y*z> data;
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
    static const int WIDTH = 16;    // x
    static const int HEIGHT = 128;  // y
    static const int BREADTH = 16;  // z

    bool re_init_vaos = true;
    bool first_vbo_init = true;

    void addToRenderMap(int blockID, int face, float lighting, glm::vec3 pos);
    void removeFromRenderMap(int face, glm::vec3 pos);

    Chunk() = default;  //for std map
    Chunk(glm::vec2 position, Array3D<WIDTH, HEIGHT, BREADTH>& blocks_array);
    void draw(Shaderprogram& shaderprogram, glm::vec3& view_dir);
    BlockInfo& getBlockInfo(int x, int y, int z);

private:
    glm::vec2 position;
    Array3D<WIDTH, HEIGHT, BREADTH> block_array;
    BlockModel block_model;
    int num_render_faces[6] = {0,0,0,0,0,0};
    std::unordered_map<glm::vec3, RenderBlockInfo, std::hash<glm::vec3>> render_faces_map[6];

    void visibiltyChecking();
    bool blockIsInChunk(int local_x, int local_y, int local_z);
    void visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID);
    void rebuildVBOs();
};

#endif // BLOCK_CHUNK_H