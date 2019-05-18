#include "../include/chunk.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Chunk::Chunk(glm::ivec2 position, const Array3D& block_array) : 
    position(position), 
    block_array(block_array),
    p_block_model(nullptr),
    first_vbo_init(true),
    re_init_vaos(true)
{
    sunlightChecking(); //add light before sending render info
    visibiltyChecking();
}


Chunk::~Chunk() //only do on main thread
{
    if(p_block_model)
    {
        delete p_block_model;
    }
}


bool Chunk::helperFunInterestingAdjacantPoint(int x, int y, int z) //returns false if out of bounds
{
    if (blockIsInChunk(x,y,z))
        if (y<sunlight_level[x][z])
            return true;
    return false;
}


void Chunk::sunlightChecking()
{
    //find sunlight level
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int z=0; z<CH_WIDTH; z++)
        {
            sunlight_level[x][z] = 0;
            for (int y=0; y<CH_HEIGHT; y++)
            {
                BlockInfo& block_info = block_array.at(x, y, z);
                block_info.lighting = 0;
                if(block_info.blockID!=0) //not air
                {
                    sunlight_level[x][z] = y+1;
                }
            }
        }
    }

    //set max sunlight values, and get the wanted BFS points.
    std::unordered_set<glm::vec3, std::hash<glm::vec3>> points;
    for (int x=0; x<CH_WIDTH; x++)
        for(int z=0; z<CH_WIDTH; z++)
            for (int y=CH_HEIGHT-1; y>-1; y--)
            {
                //direct sunlight
                BlockInfo& block_info = block_array.at(x, y, z);
                if(y>= sunlight_level[x][z])
                {
                    block_info.lighting = MAX_SUNLIGHT_VALUE;
                }
                else
                {
                    break;
                }

                //look for interesting BFS points and add to set, (with bounds checking)
                bool east = helperFunInterestingAdjacantPoint(x+1,y,z);
                bool west = helperFunInterestingAdjacantPoint(x-1,y,z);
                bool north = helperFunInterestingAdjacantPoint(x,y,z+1);
                bool south = helperFunInterestingAdjacantPoint(x,y,z-1);
                if (east || west || north || south)
                    points.insert(glm::vec3(x,y,z));
            }
    recursiveLightBFS(points);
}


void Chunk::lightBFSHelperFunc(int light_value, int x, int y, int z, std::unordered_set<glm::vec3, std::hash<glm::vec3>>& new_points)
{
    if(blockIsInChunk(x, y, z))
    {
        BlockInfo& new_block_info = block_array.at(x, y, z);
        if(new_block_info.blockID==0) //air
        {
            if (light_value > 1+new_block_info.lighting)
            {
                new_block_info.lighting = std::max(0, light_value-1);
                new_points.insert(glm::vec3(x,y,z));
            }
        }
    }
}


void Chunk::recursiveLightBFS(std::unordered_set<glm::vec3, std::hash<glm::vec3>>& points)
{
    //make one iteration at every point
    std::unordered_set<glm::vec3, std::hash<glm::vec3>> new_points;
    for(auto& p: points)
    {
        BlockInfo& block_info = block_array.at(p.x, p.y, p.z);
        int light_value = block_info.lighting;
        lightBFSHelperFunc(light_value, p.x+1, p.y,   p.z,   new_points);
        lightBFSHelperFunc(light_value, p.x-1, p.y,   p.z,   new_points);
        lightBFSHelperFunc(light_value, p.x,   p.y+1, p.z,   new_points);
        lightBFSHelperFunc(light_value, p.x,   p.y-1, p.z,   new_points);
        lightBFSHelperFunc(light_value, p.x,   p.y  , p.z+1, new_points);
        lightBFSHelperFunc(light_value, p.x,   p.y  , p.z-1, new_points);
    }

    //if new_points isnt empty, recurse
    if (!new_points.empty())
        recursiveLightBFS(new_points);
}




#include <chrono>



void Chunk::visibiltyChecking()
{
    re_init_vaos = true;
    for(int x=0; x<CH_WIDTH; x++)
    {
        for(int z=0; z<CH_WIDTH; z++)
        {   
            for(int y=0; y<CH_HEIGHT; y++)
            {
                BlockInfo& block_info = block_array.at(x, y, z);
                if(block_info.blockID!=0) //not air
                {
                    visibilityCheckingAtPos(BlockModel::TOP,    x, y, z, block_info.blockID);
                    visibilityCheckingAtPos(BlockModel::BOTTOM, x, y, z, block_info.blockID);
                    visibilityCheckingAtPos(BlockModel::NORTH,  x, y, z, block_info.blockID);
                    visibilityCheckingAtPos(BlockModel::SOUTH,  x, y, z, block_info.blockID);
                    visibilityCheckingAtPos(BlockModel::EAST,   x, y, z, block_info.blockID);
                    visibilityCheckingAtPos(BlockModel::WEST,   x, y, z, block_info.blockID);
                }
            }
        }
    }
}


void Chunk::visibilityCheckingAtPos(int face, int x, int y, int z, unsigned int blockID) //local coordinates
{   
    glm::vec3 p;
    if(face == BlockModel::EAST)
        p = glm::vec3(x+1, y, z);
    else if(face == BlockModel::WEST)
        p = glm::vec3(x-1, y, z);
    else if(face == BlockModel::TOP)
        p = glm::vec3(x, y+1, z);
    else if(face == BlockModel::BOTTOM)
        p = glm::vec3(x, y-1, z);
    else if(face == BlockModel::NORTH)
        p = glm::vec3(x, y, z+1);
    else if(face == BlockModel::SOUTH)
        p = glm::vec3(x, y, z-1);
    glm::vec3 global_pos = glm::vec3(position.x*CH_WIDTH+x, y, position.y*CH_WIDTH+z);

    removeFromRenderMap(face, global_pos);  //is this needed?
    if(blockIsInChunk(p.x, p.y, p.z))
    {   
        BlockInfo info = getBlockInfo(p.x, p.y, p.z);
        if(info.blockID == 0) //TODO TODO change to visible/transparant block
            addToRenderMap(blockID, face, info.lighting, global_pos);
    }
    else if(face == BlockModel::TOP && y == CH_HEIGHT-1)
    {
        addToRenderMap(blockID, BlockModel::TOP, MAX_SUNLIGHT_VALUE, global_pos);
    }

    

}


bool Chunk::blockIsInChunk(int x, int y, int z) //local
{
    if(x < CH_WIDTH  && x >= 0)
        if(y < CH_HEIGHT && y >= 0)
            if(z < CH_WIDTH && z >= 0)
                return true;
    return false;
}


void Chunk::addToRenderMap(int blockID, int face, int face_lighting, glm::vec3 pos)
{
    RenderBlockInfo render_info;
    render_info.blockID = blockID;
    render_info.face_lighting = face_lighting;
    render_faces_map[face][pos] = render_info;
}


void Chunk::removeFromRenderMap(int face, glm::vec3 pos)
{
    render_faces_map[face].erase(pos);
}


void Chunk::rebuildVBOs(std::array<std::unordered_map<int, int>,6>& texArrayIDLookup) //every time it is needed when a chunk is to be drawn
{
    if(first_vbo_init)
        p_block_model = new BlockModel();

    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {   
        //remove old vbos
        if(!first_vbo_init)
        {
            glDeleteBuffers(1, &p_block_model->pos_VBOs[face]);
            glDeleteBuffers(1, &p_block_model->texArrayID_VBOs[face]);
            glDeleteBuffers(1, &p_block_model->light_VBOs[face]);
        }

        //gather data for new vbos
        int size = render_faces_map[face].size();
        num_render_faces[face] = size;
        glm::vec3 positions[size];
        int texArrayIDs[size];
        int lightings[size];
        int counter = 0;
        for (auto& pair : render_faces_map[face])
        {
            glm::vec3 pos = pair.first;
            RenderBlockInfo render_info = pair.second;

            positions[counter] = pos;
            texArrayIDs[counter] = texArrayIDLookup[face][render_info.blockID];
            lightings[counter] = render_info.face_lighting;
            counter++;
        }

        //bind the data to vbos
        glBindVertexArray(p_block_model->VAOs[face]);

        glGenBuffers(1, &(p_block_model->pos_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->pos_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glGenBuffers(1, &(p_block_model->texArrayID_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->texArrayID_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), &texArrayIDs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);

        glGenBuffers(1, &(p_block_model->light_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->light_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), &lightings[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
    }
    first_vbo_init = false;
}


void Chunk::draw(Shaderprogram& shaderprogram, glm::vec3& view_dir, std::array<std::unordered_map<int, int>,6>& texArrayIDLookup)
{
    if (re_init_vaos)
    {
        rebuildVBOs(texArrayIDLookup);
        re_init_vaos = false;
    }

    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {
        glBindVertexArray(p_block_model->VAOs[face]);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, num_render_faces[face]);
    }
}


BlockInfo& Chunk::getBlockInfo(int x, int y, int z)
{
    return block_array.at(x, y, z);
}