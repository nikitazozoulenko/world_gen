#include <uiRenderer.h>
#include <misc.h>

#include <iostream>
#include <algorithm>

UIRenderer::UIRenderer(Settings& settings):
    settings(settings),
    fontDrawer(FontDrawer(settings, "/home/nikita/Code/world_gen/resources/Bitter.ttf"))
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
    else if(p_ele->type=="editorframe")
        render_editorframe(static_cast<UIFrame*>(p_ele), off_x, off_y);
    else if(p_ele->type=="xslider")
        render_slider(static_cast<UISlider*>(p_ele), off_x, off_y);
    else if(p_ele->type=="yslider")
        render_y_slider(static_cast<UIYSlider*>(p_ele), off_x, off_y);
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

void UIRenderer::render_editorframe(UIFrame* p_ele, double off_x, double off_y)
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    //window
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();
    double x = off_x+p_ele->x0;
    double y = off_y+p_ele->y0;
    double& w = p_ele->w;
    double& h = p_ele->h;

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    ui_shaderprogram.setUniformVec4("coords", x/screen_w,y/screen_h,w/screen_w,h/screen_h);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE,GL_TRUE);

    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer

    render_frame(p_ele, off_x, off_y);
    glDisable(GL_STENCIL_TEST);
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


void UIRenderer::render_y_slider(UIYSlider* p_ele, double off_x, double off_y)
{
    double& x0 = p_ele->x0;
    double& y0 = p_ele->y0;
    double& w = p_ele->w;
    double& h = p_ele->h;
    double& tick_h = p_ele->tick_h;
    double& line_w = p_ele->line_w;
    double& min = p_ele->min;
    double& max = p_ele->max;
    double& value = p_ele->value;
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();

    //tick
    double c_val_norm = (value-min)/(max-min);
    double tick_x0 = (off_x + x0);
    double tick_y0 = (off_y + y0 +c_val_norm*(h-tick_h));
    double& tick_w = w;
    ui_shaderprogram.setUniformVec4("coords", tick_x0/screen_w, tick_y0/screen_h, tick_w/screen_w, tick_h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color*0.5f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //line
    double line_x0 = (off_x +x0 + 0.5*(w - line_w));
    double line_y0 = (off_y + y0);
    double& line_h = h;
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

    ui_shaderprogram.bind();
    glBindVertexArray(vao_quad);
    ui_shaderprogram.setUniformVec4("coords", x/screen_w,y/screen_h,w/screen_w,h/screen_h);
    ui_shaderprogram.setUniformVec3("color", p_ele->color);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    std::vector<std::pair<std::string, double>> lines = fontDrawer.split(p_ele->button_text, w, 1.0f);
    //TODO check for nonempy text
    double word_w = lines[0].second;
    double word_h = fontDrawer.get_max_char_h(1.0f);
    fontDrawer.draw(lines[0].first, x + 0.5*(w-word_w), y + 0.5*(h-word_h), 1.0f, glm::vec3(1,1,1));
    ui_shaderprogram.bind();
    glBindVertexArray(vao_quad);
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