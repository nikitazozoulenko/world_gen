#include "../include/block.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

BlockModel::BlockModel()
{
    setup_VAO_VBO();
}


void BlockModel::setup_VAO_VBO()
{
    std::vector<float> top_vecrtices = {
        //vert                //normal             //texcoords
        -0.0f,  1.0f, -0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f, -0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f
    };

    std::vector<float> bottom_vecrtices = {
        -0.0f, -0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.0f, -0.0f, -0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -0.0f, -0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f
    };

    std::vector<float> north_vecrtices = {
        -0.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,
        -0.0f, -0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    1.0f,  0.0f,
         1.0f, -0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    1.0f,  1.0f
    };

    std::vector<float> south_vecrtices = {
         1.0f,  1.0f, -0.0f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f,
         1.0f, -0.0f, -0.0f,    0.0f,  0.0f, -1.0f,    0.0f,  1.0f,
        -0.0f,  1.0f, -0.0f,    0.0f,  0.0f, -1.0f,    1.0f,  0.0f,
        -0.0f, -0.0f, -0.0f,    0.0f,  0.0f, -1.0f,    1.0f,  1.0f
    };

    std::vector<float> east_vecrtices = {
         1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
         1.0f, -0.0f,  1.0f,    1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
         1.0f,  1.0f, -0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
         1.0f, -0.0f, -0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  1.0f
    };

    std::vector<float> west_vecrtices = {
        -0.0f,  1.0f, -0.0f,   -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
        -0.0f, -0.0f, -0.0f,   -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
        -0.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
        -0.0f, -0.0f,  1.0f,   -1.0f,  0.0f,  0.0f,    1.0f,  1.0f
    };

    setup_face(top_vecrtices, TOP);
    setup_face(bottom_vecrtices, BOTTOM);
    setup_face(north_vecrtices, NORTH);
    setup_face(south_vecrtices, SOUTH);
    setup_face(east_vecrtices, EAST);
    setup_face(west_vecrtices, WEST);
}


void BlockModel::setup_face(std::vector<float> vertices, int side)
{
    // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    std::cout << "BlockModel1" << std::endl;
    glGenVertexArrays(1, &(VAOs[side]));
    std::cout << "BlockModel2" << std::endl;
    glGenBuffers(1, &(VBOs[side]));
    std::cout << "BlockModel3" << std::endl;

    glBindVertexArray(VAOs[side]);
    std::cout << "BlockModel4" << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[side]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
    std::cout << "BlockModel5" << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    std::cout << "BlockModel6" << std::endl;
}