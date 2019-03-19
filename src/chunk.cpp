#include "../include/chunk.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Chunk::Chunk(glm::vec2 position, Array3D<WIDTH, HEIGHT, BREADTH> block_array)
{
    this->position = position;
    this->block_array = block_array;

    //loop through the block map and add to vector
    //the vector is used for more efficient rendering
    for(int x=0; x<WIDTH; x++)
    {
        for(int y=0; y<HEIGHT; y++)
        {
            for(int z=0; z<BREADTH; z++)
            {   
                BlockInfo block_info = block_array.at(x, y, z);
                if(block_info.visible)
                {
                    //TODO change to unordered_set insead of vector
                    this->block_locations[block_info.blockID].insert(glm::vec3(x,y,z));
                }
            }
        }
    }
}


void Chunk::draw(Shaderprogram& shaderprogram, std::unordered_map<unsigned int, BlockModel>& model_map)
{
    for (auto& pair : block_locations)
    {   
        unsigned int blockID = pair.first;
        std::unordered_set<glm::vec3> locations = pair.second;
        BlockModel block_model = model_map[blockID];

        // bind vao, diffuse and specular maps
        glBindVertexArray(block_model.VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, block_model.diffuse_map);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, block_model.specular_map);

        //model matrices for instanced rendering
        glm::mat4 model_matrices[locations.size()];
        int counter = 0;
        for (auto& pos : locations)
        {   //calculate model matrix for each object
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,glm::vec3(position.x*WIDTH, 0,  position.y*BREADTH) + pos);
            model_matrices[counter] = model;
            counter++;
        }

        unsigned int instance_buffer;
        glGenBuffers(1, &instance_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
        glBufferData(GL_ARRAY_BUFFER, locations.size() * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);

        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glDrawElementsInstanced(GL_TRIANGLES, block_model.num_indices, GL_UNSIGNED_INT, 0, locations.size());
        //remove vbo here????
        glDeleteBuffers(1, &instance_buffer);
    }
}

BlockInfo& Chunk::getBlockInfo(int x, int y, int z)
{
    return block_array.at(x, y, z);
}