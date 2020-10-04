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
    createItemTextures();
}


std::array<glm::vec4, 4> BlockRenderer::getFrustumPlanes(glm::mat4& projView)
{
    std::array<glm::vec4, 4> planes;
    // Left Frustum Plane
    planes[0].x = projView[0][3] + projView[0][0]; 
    planes[0].y = projView[1][3] + projView[1][0];
    planes[0].z = projView[2][3] + projView[2][0];
    planes[0].w = projView[3][3] + projView[3][0];

    // Right Frustum Plane
    planes[1].x = projView[0][3] - projView[0][0]; 
    planes[1].y = projView[1][3] - projView[1][0];
    planes[1].z = projView[2][3] - projView[2][0];
    planes[1].w = projView[3][3] - projView[3][0];

    // Top Frustum Plane
    planes[2].x = projView[0][3] - projView[0][1]; 
    planes[2].y = projView[1][3] - projView[1][1];
    planes[2].z = projView[2][3] - projView[2][1];
    planes[2].w = projView[3][3] - projView[3][1];

    // Bottom Frustum Plane
    planes[3].x = projView[0][3] + projView[0][1];
    planes[3].y = projView[1][3] + projView[1][1];
    planes[3].z = projView[2][3] + projView[2][1];
    planes[3].w = projView[3][3] + projView[3][1];
    return planes;
}


bool BlockRenderer::frustrumCulling(glm::ivec2& chunk_pos, std::array<glm::vec4, 4>& planes, float& sphere_r)
{   
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();

    //second: foreach plane, signed distance
    for(glm::vec4& plane : planes){
        glm::vec3 p1 = glm::vec3((chunk_pos.x+0.5)*ch_width, 0, (chunk_pos.y+0.5)*ch_depth);
        glm::vec3 p2 = glm::vec3(p1.x, p1.y+ch_height, p1.z);
        float d1 = glm::dot(glm::vec3(plane.x, plane.y, plane.z), p1) + plane.w;
        float d2 = glm::dot(glm::vec3(plane.x, plane.y, plane.z), p2) + plane.w;
        float d = glm::max(d1, d2);
        if(d<-sphere_r){
            return false;
        }
    }
    return true;

}


void BlockRenderer::render(ChunkMapivec2& chunk_map, Camera& camera)
{
    //bind shaders
    block_shaderprogram.bind();

    //update projview matrix every frame
    glm::mat4 projView = getProjViewMatrix(camera);
    setProjViewMatrix(projView);

    //render cubes
    std::array<glm::vec4, 4> planes = getFrustumPlanes(projView);
    float sphere_r = glm::sqrt(settings.getChunkWidth()*settings.getChunkWidth() + settings.getChunkWidth()*settings.getChunkWidth());
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture);
    for (auto& pair : chunk_map)
    {
        //TODO: frustrum culling?
        if(frustrumCulling(pair.second.pos, planes, sphere_r)){
            pair.second.render(texArrayIDLookup);
        }
    }
}

glm::mat4 BlockRenderer::getProjViewMatrix(Camera& camera)
{
    glm::mat4 proj = glm::perspective(glm::radians(camera.zoom), (float)settings.getWindowWidth() / (float)settings.getWindowHeight(), settings.zNear, settings.zFar);
    glm::mat4 view = camera.getViewMatrix();
    return proj*view;
}

void BlockRenderer::setProjViewMatrix(glm::mat4& projView)
{
    block_shaderprogram.setUniformMat4("projView", projView);
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
    else if(j["parent"] == "grass")
    {
        std::vector<int> top= {BlockModel::TOP};
        std::vector<int> side= {BlockModel::EAST, BlockModel::WEST, BlockModel::NORTH, BlockModel::SOUTH};
        std::vector<int> bot= {BlockModel::BOTTOM};
        addFacesWithTexture(j["textures"]["top"], top, pathToIndexMap, tex_dir);
        addFacesWithTexture(j["textures"]["bot"], bot, pathToIndexMap, tex_dir);
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


void BlockRenderer::createItemTextures()
{
    for(auto& pair : blockIDMap){
        std::string name = pair.first;
        unsigned int blockID = pair.second;
        itemTexMap[blockID] = blockID;
    }
}