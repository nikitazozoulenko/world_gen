#include "../include/uiRenderer.h"

#include <iostream>
#include "../include/misc.h"

UIRenderer::UIRenderer(GameWorld* p_game_world)
{
    this->p_game_world = p_game_world;
    createShaders();
    createUI();
}


void UIRenderer::render()
{
    //bind shaders
    ui_shaderprogram.use();

    //render here
    for (auto& quad : quads)
    {
        quad.draw(ui_shaderprogram);
    }
}


void UIRenderer::createShaders()
{
    ui_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/ui.vs", "/home/nikita/Code/world_gen/src/shaders/ui.fs");
}


void UIRenderer::createUI()
{
    float size = 0.1f;
    quads.push_back(UIQuad(-size/16, size/16, -size/9, size/9, "crosshair"));
}


UIQuad::UIQuad(float x0, float x1, float y0, float y1, std::string filename)
{
    setup(x0, x1, y0, y1, filename);
}


void UIQuad::setup(float x0, float x1, float y0, float y1, std::string filename)
{
    float vertices[] = {
        //pos         //tex coords
        x0, y0, 0.0f, 0.0f, 0.0f,
        x1, y0, 0.0f, 1.0f, 0.0f,
        x1, y1, 0.0f, 1.0f, 1.0f,
        x0, y0, 0.0f, 0.0f, 0.0f,
        x1, y1, 0.0f, 1.0f, 1.0f,
        x0, y1, 0.0f, 0.0f, 1.0f
    };

        // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // load textures 
    texture = loadTexture(("/home/nikita/Code/world_gen/resources/ui/"+filename+".png").c_str());
}


void UIQuad::draw(Shaderprogram& shaderprogram)
{
    // bind vao, texture
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
