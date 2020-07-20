#include <blockRenderer.h>
#include <misc.h>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

// Include standard headers
#include <fstream>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>

BlockRenderer::BlockRenderer(Settings& settings) :
    settings(settings)
{
    createShaders();
    setupBlockData();
}

void BlockRenderer::render(ChunkMapivec2& chunk_map, Camera& camera)
{
    //bind shaders
    block_shaderprogram.bind();

    //update view matrix every frame   (proj matrix really should NOT be set every frame)
    setProjectionMatrix(camera);
    setViewMatrix(camera);

    //render cubes
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture);
    for (auto& pair : chunk_map)
    {
        //TODO: frustrum culling?
        pair.second.render(texArrayIDLookup);
    }
}


void BlockRenderer::setProjectionMatrix(Camera& camera)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)settings.getWindowWidth() / (float)settings.getWindowHeight(), 0.1f, 1000.0f);
    block_shaderprogram.setUniformMat4("projection", projection);
}


void BlockRenderer::setViewMatrix(Camera& camera)
{
    glm::mat4 view = camera.getViewMatrix();
    block_shaderprogram.setUniformMat4("view", view);
}


void BlockRenderer::createShaders()
{   
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/block.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/block.frag";
    const char * compute_path = nullptr;
    block_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
    block_shaderprogram.setUniformVec3("sun_dir", glm::vec3(1,2,3));
}


void BlockRenderer::addFacesWithTexture(std::string tex_path, std::vector<int> faces, 
    std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir)
{
    //index
    tex_path = tex_dir+tex_path;
    int index;
    if(pathToIndexMap.count(tex_path) > 0){
        index = pathToIndexMap[tex_path];
    }else{
        index = pathToIndexMap.size();
        pathToIndexMap[tex_path] = index;
    }

    //same for all faces
    for(int& face : faces){
        texArrayIDLookup[face][blockIDMap.size()] = index;
    }
}


void BlockRenderer::readBlockJson(nlohmann::json& j, std::unordered_map<std::string, unsigned int>& pathToIndexMap, std::string& tex_dir)
{
    //name and blockID
    int n = blockIDMap.size();
    blockIDMap[j["name"]] = n+1;

    //cube (ex dirt)
    if(j["parent"] == "cube")
    {
        std::vector<int> side= {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
        addFacesWithTexture(j["textures"]["side"], side, pathToIndexMap, tex_dir);
    }
    //column (ex log)
    else if(j["parent"] == "column")
    {
        std::vector<int> end= {BlockModel::TOP, BlockModel::BOTTOM};
        std::vector<int> side= {BlockModel::EAST, BlockModel::WEST, BlockModel::NORTH, BlockModel::SOUTH};
        addFacesWithTexture(j["textures"]["end"], end, pathToIndexMap, tex_dir);
        addFacesWithTexture(j["textures"]["side"],side, pathToIndexMap, tex_dir);
    }
}


void BlockRenderer::setupBlockData()
{
    blockIDMap["Air"] = 0;
    std::unordered_map<std::string, unsigned int> pathToIndexMap;
    std::string jsons_folder = "/home/nikita/Code/world_gen/blocks";
    std::string tex_dir = "/home/nikita/Code/world_gen/resources/Jicklus/";

    try{
        if (fs::exists(jsons_folder)){
            for (fs::directory_entry& entry : fs::directory_iterator(jsons_folder))
            {
                std::ifstream strm(entry.path().string());
                nlohmann::json j;
                strm >> j;
                readBlockJson(j, pathToIndexMap, tex_dir);
            }

        }
    }
    catch (const fs::filesystem_error& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    int image_width = 16;
    block_texture = loadTextureArray(pathToIndexMap, image_width);
}