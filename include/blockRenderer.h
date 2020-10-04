#ifndef MY_BLOCKRENDERER_H
#define MY_BLOCKRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include <unordered_map>
#include <string>
#include <array>

#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>
#include <chunk.h>
#include <world.h>


class BlockRenderer
{
public:
    BlockRenderer(Settings& settings);
    void render(ChunkMapivec2& chunk_map, Camera& camera);
    std::array<std::unordered_map<int, int>,6> texArrayIDLookup;
    std::unordered_map<std::string, unsigned int> blockIDMap;
    std::unordered_map<unsigned int, unsigned int> itemTexMap;
    
private:
    Settings& settings;
    Shaderprogram block_shaderprogram;
    unsigned int block_texture;

    void createShaders();

    void createItemTextures();

    void addFacesWithTexture(std::string tex_path, std::vector<int> faces, std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir);
    void readBlockJson(nlohmann::json& j, std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir);
    void setupBlockData();

    glm::mat4 getProjViewMatrix(Camera& camera);
    void setProjViewMatrix(glm::mat4& projView);

    std::array<glm::vec4, 4> getFrustumPlanes(glm::mat4& projView);
    bool frustrumCulling(glm::ivec2& chunk_pos, std::array<glm::vec4, 4>& planes, float& sphere_r);

    unsigned int item_vao;
    unsigned int item_vbo;
};
#endif // MY_BLOCKRENDERER_H