#include "../include/chunk.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Chunk::Chunk(glm::ivec2 position) : 
    position(position)
{
    block_array = Array3D();
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int y=0; y<CH_HEIGHT; y++)
        {
            for (int z=0; z<CH_WIDTH; z++)
            {
                int& block = getBlock(x,y,z);
                if (y<x+z)
                    block = 1;
                else
                    block = 0;
            }
        }
    }
}


int Chunk::buildVBOs(BlockModel& block_model)
{
    std::vector<glm::vec3> positions;
    std::vector<int> blocks;
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int y=0; y<CH_HEIGHT; y++)
        {
            for (int z=0; z<CH_WIDTH; z++)
            {
                int block = getBlock(x,y,z);
                if(block != 0) //is block
                {
                    positions.push_back(glm::vec3(x+CH_WIDTH*position.x, y, z+CH_WIDTH*position.y));
                    blocks.push_back(block);
                }
            }
        }
    }
    int size = positions.size();

    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {   
        //bind the data to vbos
        glBindVertexArray(block_model.VAOs[face]);

        glGenBuffers(1, &(block_model.pos_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, block_model.pos_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glGenBuffers(1, &(block_model.block_VBOs[face]));
        glBindBuffer(GL_ARRAY_BUFFER, block_model.block_VBOs[face]);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), &blocks[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
    }
    return size;
}


void Chunk::draw(Shaderprogram& shaderprogram, BlockModel& block_model)
{
    int size = buildVBOs(block_model);
    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
    for(int& face : faces)
    {
        glBindVertexArray(block_model.VAOs[face]);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, size);
    }
}












int& Chunk::getBlock(int x, int y, int z)
{
    return block_array.at(x, y, z);
}