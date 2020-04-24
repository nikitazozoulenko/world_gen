#include <uiRenderer.h>
#include <uiWindow.h>
#include <misc.h>

#include <iostream>
#include <algorithm>

UIRenderer::UIRenderer(Settings& settings):
    settings(settings),
    fontDrawer(FontDrawer("/home/nikita/Code/world_gen/resources/KratosTrueType.ttf"))
{
    createShaders();
    create_quad_vao_vbo();
}



void UIRenderer::render(std::vector<UIWindow*>& ui_windows)
{
    //bind shaders
    ui_shaderprogram.bind();
    glBindVertexArray(vao_quad);

    //render windows in the right order
    std::sort(ui_windows.begin(), ui_windows.end(), [](const auto& lhs, const auto& rhs){return lhs->time_last_press > rhs->time_last_press;});
    for (auto& p_ui_window : ui_windows)
    {
        render_window(p_ui_window);
    }

    //render text
    fontDrawer.draw("My Test", 25.0f, 25.0f, 1.0f, glm::vec3(1,1,1));
}



void UIRenderer::render_window(UIWindow* p_ui_window)
{
    for(auto& slider : p_ui_window->sliders){
        render_window_slider(slider, p_ui_window);
    }
    //window
    ui_shaderprogram.setUniformVec4("coords", p_ui_window->coords.x, p_ui_window->coords.y, p_ui_window->width, p_ui_window->height);
    ui_shaderprogram.setUniformVec3("color", p_ui_window->color);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}



void UIRenderer::render_window_slider(UISlider& slider, UIWindow* p_ui_window)
{
    glm::vec2& win_coords = p_ui_window->coords;
    float& win_width = p_ui_window->width;
    float& win_height = p_ui_window->height;
    glm::vec3& color = p_ui_window->color;

    //local (slider space is whole [0,1])
    float& min = slider.min;
    float& max = slider.max;
    float& value = slider.value;

    float tick_w = slider.tick_width;
    float line_h = slider.line_height;

    float c_val_norm = (value-min)/(max-min);
    float line_y0 = 0.5 - line_h/2.0;
    float line_x0 = 0;
    float tick_y0 = 0;
    float tick_x0 = c_val_norm * (1-tick_w);

    //local in window
    glm::vec2& coords = slider.coords;
    float& w = slider.width;
    float& h = slider.height;
    line_y0 = line_y0*h + coords.y;
    line_x0 = line_x0*w + coords.x;
    line_h = line_h*h;
    tick_y0 = tick_y0*h + coords.y;
    tick_x0 = tick_x0*w + coords.x;
    tick_w = tick_w*w;

    //global
    line_y0 = line_y0*win_height + win_coords.y;
    line_x0 = line_x0*win_width + win_coords.x;
    line_h = line_h*win_height;
    float line_w = w * win_width;
    tick_y0 = tick_y0*win_height + win_coords.y;
    tick_x0 = tick_x0*win_width + win_coords.x;
    tick_w = tick_w*win_width;
    float tick_h = h * win_height;


    //tick
    ui_shaderprogram.setUniformVec4("coords", tick_x0, tick_y0, tick_w, tick_h);
    ui_shaderprogram.setUniformVec3("color", color*0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //line
    ui_shaderprogram.setUniformVec4("coords", line_x0, line_y0, line_w, line_h);
    ui_shaderprogram.setUniformVec3("color", color*0.75f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}



void UIRenderer::createShaders()
{
    const char * vertex_path =   "/home/nikita/Code/world_gen/src/shaders/ui.vert";
    const char * geometry_path = nullptr;
    const char * fragment_path = "/home/nikita/Code/world_gen/src/shaders/ui.frag";
    const char * compute_path =  nullptr;

    ui_shaderprogram = Shaderprogram(vertex_path, geometry_path, fragment_path, compute_path);
}



void UIRenderer::create_quad_vao_vbo()
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

    // first, configure the squares's VAO (and VBO)
    glGenVertexArrays(1, &vao_quad);
    glGenBuffers(1, &vbo_quad);

    glBindVertexArray(vao_quad);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}


// void UIRenderer::add_ui_window(UIWindow* p_ui_window)
// {
//     float vertices[] = {
//         //pos         //tex coords
//         -1, -1, 0.0f, 0.0f, 0.0f,
//          1, -1, 0.0f, 1.0f, 0.0f,
//          1,  1, 0.0f, 1.0f, 1.0f,
//         -1, -1, 0.0f, 0.0f, 0.0f,
//          1,  1, 0.0f, 1.0f, 1.0f,
//         -1,  1, 0.0f, 0.0f, 1.0f
//     };

//     // first, configure the squares's VAO (and VBO)
//     glGenVertexArrays(1, &VAOS[p_ui_window]);
//     glGenBuffers(1, &VBOS[p_ui_window]);

//     glBindVertexArray(VAOS[p_ui_window]);

//     glBindBuffer(GL_ARRAY_BUFFER, VBOS[p_ui_window]);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
//     glEnableVertexAttribArray(1);
// }


// void UIRenderer::remove_ui_window(UIWindow* p_ui_window)
// {

// }