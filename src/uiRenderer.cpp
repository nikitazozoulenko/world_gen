#include <uiRenderer.h>
#include <misc.h>

#include <iostream>
#include <algorithm>

UIRenderer::UIRenderer(Settings& settings):
    settings(settings),
    fontDrawer(FontDrawer(settings, "/home/nikita/Code/world_gen/resources/KratosTrueType.ttf"))
{
    createShaders();
    create_quad_vao_vbo();
}



void UIRenderer::render(UI* p_ui)
{
    print_float("render ui", 0);
    //bind shaders
    ui_shaderprogram.bind();
    glBindVertexArray(vao_quad);

    //render windows in the right order
    std::vector<UIElement*>& elements = p_ui->elements;
    std::sort(elements.begin(), elements.end(), [](const auto& lhs, const auto& rhs){return lhs->time_last_press > rhs->time_last_press;});
    for (auto& p_ele : elements)
    {
        render_element(p_ele, 0, 0);
    }
}


void UIRenderer::render_element(UIElement* p_ele, double off_x, double off_y)
{
    if(p_ele->type=="frame")
        render_frame(static_cast<UIFrame*>(p_ele), off_x, off_y);
    else if(p_ele->type=="slider")
        render_slider(static_cast<UISlider*>(p_ele), off_x, off_y);
    else if(p_ele->type=="button")
        render_button(static_cast<UIButton*>(p_ele), off_x, off_y);
    else
        print_float("failed to render p_ele", 0);
}


void UIRenderer::render_frame(UIFrame* p_ele, double off_x, double off_y)
{
    //window
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();
    double x = off_x+p_ele->x0;
    double y = off_y+p_ele->y0;
    double& w = p_ele->w;
    double& h = p_ele->h;

    for(auto& p_child : p_ele->children){
        render_element(p_child, x, y);
    }
    ui_shaderprogram.setUniformVec4("coords", x/screen_w,y/screen_h,w/screen_w,h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void UIRenderer::render_slider(UISlider* p_ele, double off_x, double off_y)
{
    double& x0 = p_ele->x0;
    double& y0 = p_ele->y0;
    double& w = p_ele->w;
    double& h = p_ele->h;
    double& tick_w = p_ele->tick_w;
    double& line_h = p_ele->line_h;
    double& min = p_ele->min;
    double& max = p_ele->max;
    double& value = p_ele->value;
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();

    //tick
    double c_val_norm = (value-min)/(max-min);
    double tick_x0 = (off_x + x0 + c_val_norm*(w-tick_w));
    double tick_y0 = (off_y + y0);
    double& tick_h = h;
    ui_shaderprogram.setUniformVec4("coords", tick_x0/screen_w, tick_y0/screen_h, tick_w/screen_w, tick_h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color*0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //line
    double line_x0 = (off_x +x0);
    double line_y0 = (off_y + y0 + 0.5*(h - line_h));
    double& line_w = w;
    ui_shaderprogram.setUniformVec4("coords", line_x0/screen_w, line_y0/screen_h, line_w/screen_w, line_h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color*0.75f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void UIRenderer::render_button(UIButton* p_ele, double off_x, double off_y)
{
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();
    double x = off_x+p_ele->x0;
    double y = off_y+p_ele->y0;
    double& w = p_ele->w;
    double& h = p_ele->h;

    std::vector<std::pair<std::string, double>> lines = fontDrawer.split(p_ele->button_text, w, 1.0f);
    //TODO check for nonempy text
    double word_w = lines[0].second;
    fontDrawer.draw(lines[0].first, x + 0.5*(w-word_w), y, 1.0f, glm::vec3(1,1,1));
    //TODO find out how high the text is to make it centered

    ui_shaderprogram.bind();
    glBindVertexArray(vao_quad);
    ui_shaderprogram.setUniformVec4("coords", x/screen_w,y/screen_h,w/screen_w,h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// void UIRenderer::render_window(UIWindow* p_ui_window)
// {
//     for(auto& slider : p_ui_window->sliders){
//         render_window_slider(slider, p_ui_window);
//     }
//     //window
//     double screen_w = settings.getWindowWidth();
//     double screen_h = settings.getWindowHeight();
//     double x = p_ui_window->x0/screen_w;
//     double y = p_ui_window->y0/screen_h;
//     double w = p_ui_window->w/screen_w;
//     double h = p_ui_window->h/screen_h;
//     ui_shaderprogram.setUniformVec4("coords", x,y,w,h);
//     ui_shaderprogram.setUniformVec3("color", p_ui_window->color);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
// }



// void UIRenderer::render_window_slider(UISlider& slider, UIWindow* p_ui_window)
// {   //sizes in pixels
//     double& win_x0 = p_ui_window->x0;
//     double& win_y0 = p_ui_window->y0;
//     double& win_w = p_ui_window->w;
//     double& win_h = p_ui_window->h;
//     glm::vec3& color = p_ui_window->color;
//     double& min = slider.min;
//     double& max = slider.max;
//     double& value = slider.value;
//     double screen_w = settings.getWindowWidth();
//     double screen_h = settings.getWindowHeight();

//     //tick
//     double c_val_norm = (value-min)/(max-min);
//     double tick_x0 = (win_x0 + slider.x0 + c_val_norm*(slider.w-slider.tick_w)) / screen_w;
//     double tick_y0 = (win_y0 + slider.y0)/screen_h;
//     double tick_w = slider.tick_w/screen_w;
//     double tick_h = slider.h/screen_h;
//     ui_shaderprogram.setUniformVec4("coords", tick_x0, tick_y0, tick_w, tick_h);
//     ui_shaderprogram.setUniformVec3("color", color*0.5f);
//     glDrawArrays(GL_TRIANGLES, 0, 6);

//     //line
//     double line_x0 = (win_x0 + slider.x0)/screen_w;
//     double line_y0 = (win_y0 + slider.y0 + 0.5*(slider.h - slider.line_h)) / screen_h;
//     double line_w = slider.w/screen_w;
//     double line_h = slider.line_h/screen_h;
//     ui_shaderprogram.setUniformVec4("coords", line_x0, line_y0, line_w, line_h);
//     ui_shaderprogram.setUniformVec3("color", color*0.75f);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
// }



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