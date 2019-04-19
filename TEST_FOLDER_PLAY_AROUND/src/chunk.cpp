#include "../include/chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


Chunk::Chunk(glm::vec2 position, const Array3D& block_array) : 
    position(position), 
    block_array(block_array)
{
    std::cout << "chunk1" << std::endl;
    std::cout << "chunk2" << std::endl;
}