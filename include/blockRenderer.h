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
    
private:
    Settings& settings;
    Shaderprogram block_shaderprogram;
    unsigned int block_texture;

    void createShaders();

    void addFacesWithTexture(std::string tex_path, std::vector<int> faces, std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir);
    void readBlockJson(nlohmann::json& j, std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir);
    void setupBlockData();
    unsigned int singleFaceBlock(int id, const char * path, std::unordered_map<std::string, unsigned int>& pathToIndexMap);

    void setProjectionMatrix(Camera& camera);
    void setViewMatrix(Camera& camera);
};
#endif // MY_BLOCKRENDERER_H