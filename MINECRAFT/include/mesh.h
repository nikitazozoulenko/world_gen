#ifndef MY_MESH_H
#define MY_MESH_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "shaderprogram.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    unsigned int ID;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    //Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
    void draw(Shaderprogram& shaderprogram);
private:
    void setupMesh();
};
#endif // MY_MESH_H