#include <uiRenderer.h>
#include <misc.h>

#include <iostream>
#include <algorithm>

UIRenderer::UIRenderer():
    fontDrawer(FontDrawer("/home/nikita/Code/world_gen/resources/KratosTrueType.ttf"))
{
    createShaders();
}


void UIRenderer::render(std::vector<UIWindow>& ui_windows)
{
    //bind shaders
    ui_shaderprogram.bind();

    //render windows in the right order
    std::sort(ui_windows.begin(), ui_windows.end(), [](const auto& lhs, const auto& rhs){return lhs.last_clicked > rhs.last_clicked;});
    for (auto& ui_window : ui_windows)
    {
        // bind vao, texture
        ui_shaderprogram.setUniformVec4("coords", ui_window.coords.x, ui_window.coords.y, ui_window.width, ui_window.height);
        ui_shaderprogram.setUniformVec3("color", ui_window.color);
        ui_shaderprogram.setUniformFloat("smoothing_r", ui_window.smoothing_r);
        glBindVertexArray(ui_window.VAO);
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


