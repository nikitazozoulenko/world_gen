#include "../include/block.h"
#include "../include/misc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

BlockModel::BlockModel(std::string folder)
{
    if (folder == "grass")
        setup_side_cube(folder);
    else
        setup_normal_cube(folder);
}

BlockModel::BlockModel() //WARNING, CONSTRUCTOR ONLY FOR UNORDERED_MAP
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    num_indices = 0;
    diffuse_map = 0;
    specular_map = 0;

}

BlockModel::BlockModel(const BlockModel& obj)
{
    VAO = obj.VAO;
    VBO = obj.VBO;
    EBO = obj.EBO;
    num_indices = obj.num_indices;
    diffuse_map = obj.diffuse_map;
    specular_map = obj.specular_map;
}


void BlockModel::setup_side_cube(std::string folder)
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions            // normals             // texture coords
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.333333333333f, //side
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.666666666666f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.666666666666f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.333333333333f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.333333333333f, //side
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.666666666666f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.666666666666f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.333333333333f,

        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.333333333333f, //side
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.666666666666f,
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.666666666666f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.333333333333f,

         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.333333333333f, //side
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.666666666666f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.666666666666f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.333333333333f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  //bottom
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.333333333333f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.333333333333f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.666666666666f,  //top
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.666666666666f
    };

    unsigned int indices[] = {
        0,   2,  1,  0,  3,  2,   //sides
        4,   6,  5,  4,  7,  6,
        8,  10,  9,  8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,  //bottom
        20, 22, 21, 20, 23, 22   //top
    };
    num_indices = sizeof(indices) / sizeof( indices[0] );

    // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load textures 
    diffuse_map = loadTexture(("/home/nikita/Code/world_gen/resources/blocks/"+folder+"/texture.png").c_str());
    specular_map = loadTexture(("/home/nikita/Code/world_gen/resources/blocks/"+folder+"/specular.png").c_str());
}


void BlockModel::setup_normal_cube(std::string folder)
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions            // normals             // texture coords
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f, //side
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0, //side
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0,

        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0, //side
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0,

         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0, //side
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  //bottom
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0,  //top
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0
    };

    unsigned int indices[] = {
        0,   2,  1,  0,  3,  2,   //sides
        4,   6,  5,  4,  7,  6,
        8,  10,  9,  8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,  //bottom
        20, 22, 21, 20, 23, 22   //top
    };
    num_indices = sizeof(indices) / sizeof( indices[0] );

    // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load textures 
    diffuse_map = loadTexture(("/home/nikita/Code/world_gen/resources/blocks/"+folder+"/texture.png").c_str());
    specular_map = loadTexture(("/home/nikita/Code/world_gen/resources/blocks/"+folder+"/specular.png").c_str());
}