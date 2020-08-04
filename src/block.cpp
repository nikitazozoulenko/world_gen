#include <block.h>
#include <misc.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

BlockModel::BlockModel()
{
    setup_VAO_VBO();
}


BlockModel::~BlockModel() //can only be done on main thread?
{
    if(!first_vbo_init){
        for(int i=0; i<6; i++)
        {
            glDeleteVertexArrays(1, &VAOs[i]);
            glDeleteBuffers(1, &VBOs[i]);
            glDeleteBuffers(1, &pos_VBOs[i]);
            glDeleteBuffers(1, &texArrayID_VBOs[i]);
        }
    }
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

    setup_face(east_vecrtices, EAST);
    setup_face(west_vecrtices, WEST);
    setup_face(top_vecrtices, TOP);
    setup_face(bottom_vecrtices, BOTTOM);
    setup_face(north_vecrtices, NORTH);
    setup_face(south_vecrtices, SOUTH);
}


void BlockModel::setup_face(std::vector<float> vertices, int side)
{
    // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &(VAOs[side]));
    glGenBuffers(1, &(VBOs[side]));

    glBindVertexArray(VAOs[side]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[side]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}