#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include <glm/glm.hpp>

#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>

#include <unordered_map>
#include <vector>

class Chunk
{
public:
    Chunk(Settings& settings, glm::ivec2 pos);
    ~Chunk();
    void render();
    glm::vec4 getData(int x, int y, int z);
    glm::vec4 getData(glm::ivec3 v);
    void setData(float value, int x, int y, int z);
    
private:
    Settings& settings;
    glm::ivec2 pos;
    float surface_level;
    float* data;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    unsigned int VAO;
    unsigned int vertex_VBO;
    unsigned int normal_VBO;

    void setUp();
    void marchingCube(int x, int y, int z);
};
#endif // WORLD_CHUNK_H