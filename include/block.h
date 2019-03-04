#ifndef BLOCK_CHUNK_H
#define BLOCK_CHUNK_H

#include <glm/glm.hpp>

#include <unordered_map>

class Block
{
public:
    Block();
    void draw(Shaderprogram& shaderprogram);
    void setup();
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int diffuse_map;
    unsigned int specular_map;
};


class Chunk
{
public:
    glm::vec3 position;
    // static uint WIDTH = 32;
    // static uint HEIGHT = 64;
    // Block blocks[WIDTH][WIDTH][HEIGHT];
    Chunk(glm::vec3 position);
    std::unordered_map<glm::vec3, Block> block_map;

    void draw(Shaderprogram& shaderprogram);
private:
};

#endif // BLOCK_CHUNK_H