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


void Chunk::draw(Shaderprogram& shaderprogram, BlockModel& block_model)
{
    for (int x=0; x<CH_WIDTH; x++)
    {
        for (int y=0; y<CH_HEIGHT; y++)
        {
            for (int z=0; z<CH_WIDTH; z++)
            {
                int block = getBlock(x,y,z);
                if(block != 0) //is block
                {
                    //uniforms
                    shaderprogram.setUniformVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x+position.x*CH_WIDTH, y, z+position.y*CH_WIDTH));
                    shaderprogram.setUniformMat4("model", model);

                    //render single block
                    int faces[6] = {BlockModel::EAST, BlockModel::WEST, BlockModel::TOP, BlockModel::BOTTOM, BlockModel::NORTH, BlockModel::SOUTH};
                    for(int& face : faces)
                    {
                        glBindVertexArray(block_model.VAOs[face]);
                        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    }
                }
            }
        }
    }
}


int& Chunk::getBlock(int x, int y, int z)
{
    return block_array.at(x, y, z);
}