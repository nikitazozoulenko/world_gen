#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include <glm/glm.hpp>
#include <unordered_map>
#include "shaderprogram.h"
#include "camera.h"

#include <vector>

#define CH_WIDTH 32
#define CH_HEIGHT 32
#define CH_DEPTH 32

class Chunk
{
public:
    Chunk(glm::ivec2 pos);
    void render();
    glm::vec4 getData(int x, int y, int z);
    glm::vec4 getData(glm::ivec3 v);
    void setData(float value, int x, int y, int z);
    
private:
    glm::ivec2 pos;
    float surface_level;
    float data[(CH_WIDTH+1) * (CH_HEIGHT) * (CH_DEPTH+1) * 4];
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    unsigned int VAO;
    unsigned int vertex_VBO;
    unsigned int normal_VBO;


    void setUp();
    void marchingCube(int x, int y, int z);
};
#endif // WORLD_CHUNK_H