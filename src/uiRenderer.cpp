#include <uiRenderer.h>
#include <uiWindow.h>
#include <misc.h>

#include <iostream>
#include <algorithm>

UIRenderer::UIRenderer():
    fontDrawer(FontDrawer("/home/nikita/Code/world_gen/resources/KratosTrueType.ttf"))
{
    createShaders();
}


void UIRenderer::render(std::vector<UIWindow*>& ui_windows)
{
    //bind shaders
    ui_shaderprogram.bind();

    //render windows in the right order
    std::sort(ui_windows.begin(), ui_windows.end(), [](const auto& lhs, const auto& rhs){return lhs->time_last_press > rhs->time_last_press;});
    for (auto& p_ui_window : ui_windows)
    {
        // bind vao, texture
        ui_shaderprogram.setUniformVec4("coords", p_ui_window->coords.x, p_ui_window->coords.y, p_ui_window->width, p_ui_window->height);
        ui_shaderprogram.setUniformVec3("color", p_ui_window->color);
        glBindVertexArray(VAOS[p_ui_window]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //render text
    fontDrawer.draw("My Test", 25.0f, 25.0f, 1.0f, glm::vec3(1,1,1));
}


void UIRenderer::createShaders()
{
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/ui.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/ui.frag";
    const char * compute_path =  nullptr;

    ui_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
}


void UIRenderer::add_ui_window(UIWindow* p_ui_window)
{
    float vertices[] = {
        //pos         //tex coords
        -1, -1, 0.0f, 0.0f, 0.0f,
         1, -1, 0.0f, 1.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1, -1, 0.0f, 0.0f, 0.0f,
         1,  1, 0.0f, 1.0f, 1.0f,
        -1,  1, 0.0f, 0.0f, 1.0f
    };

    // first, configure the cube's VAO (and VBO)         //NOTE: all cubes have same VAO, (also same vbo? dunno)
    glGenVertexArrays(1, &VAOS[p_ui_window]);
    glGenBuffers(1, &VBOS[p_ui_window]);

    glBindVertexArray(VAOS[p_ui_window]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS[p_ui_window]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}


void UIRenderer::remove_ui_window(UIWindow* p_ui_window)
{
    //TODO FREE DATA FROM BUFFERS TODO TODO TODO

}