#include "../include/chunk.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Chunk::Chunk(glm::vec2 position, const Array3D& block_array) : 
    position(position), 
    block_array(block_array),
    p_block_model(nullptr),
    first_vbo_init(true),
    re_init_vaos(true)
{
    visibiltyChecking();
}


Chunk::~Chunk() //only do on main thread
{
    if(p_block_model)
    {
        delete p_block_model;
    }
}


void Chunk::visibiltyChecking()
{
    for(int x=0; x<CH_WIDTH; x++)
    {
        for(int y=0; y<CH_HEIGHT; y++)
        {
            for(int z=0; z<CH_WIDTH; z++)
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

    if(blockIsInChunk(p.x, p.y, p.z))
    {   
        BlockInfo info = getBlockInfo(p.x, p.y, p.z);
        if(info.blockID == 0) //TODO TODO change to visible/transparant block
            addToRenderMap(blockID, face, info.lighting, glm::vec3(position.x*CH_WIDTH+x, y, position.y*CH_WIDTH+z));
    }
    else if(face == BlockModel::TOP && y == CH_HEIGHT-1)
        //max sunlight TODO TODO TODO TODO TODO TODO
        addToRenderMap(blockID, BlockModel::TOP, 1.0f, glm::vec3(position.x*CH_WIDTH+x, y, position.y*CH_WIDTH+z));
}


bool Chunk::blockIsInChunk(int x, int y, int z) //local
{
    if(x < CH_WIDTH  && x >= 0)
        if(y < CH_HEIGHT && y >= 0)
            if(z < CH_WIDTH && z >= 0)
                return true;
    return false;
}


void Chunk::addToRenderMap(int blockID, int face, float face_lighting, glm::vec3 pos)
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
            glDeleteBuffers(1, &p_block_model->mat_VBOs[face]);
            glDeleteBuffers(1, &p_block_model->texArrayID_VBOs[face]);
            glDeleteBuffers(1, &p_block_model->light_VBOs[face]);
        }

        //gather data for new vbos
        int size = render_faces_map[face].size();
        num_render_faces[face] = size;
        glm::mat4 model_matrices[size];
        int texArrayIDs[size];
        float lightings[size];
        int counter = 0;
        for (auto& pair : render_faces_map[face])
        {
            glm::vec3 pos = pair.first;
            RenderBlockInfo render_info = pair.second;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,pos);
            model_matrices[counter] = model;

            texArrayIDs[counter] = texArrayIDLookup[face][render_info.blockID];
            lightings[counter] = render_info.face_lighting;
            counter++;
        }

        //bind the data to vbos
        glBindVertexArray(p_block_model->VAOs[face]);

        glGenBuffers(1, &(p_block_model->mat_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->mat_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);
        // set attribute pointers (matrix 4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glGenBuffers(1, &(p_block_model->texArrayID_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->texArrayID_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), &texArrayIDs[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(7);
        glVertexAttribIPointer(7, 1, GL_INT, sizeof(int), (void*)0);

        glGenBuffers(1, &(p_block_model->light_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, p_block_model->light_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), &lightings[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
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