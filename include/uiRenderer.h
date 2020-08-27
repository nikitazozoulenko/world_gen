#ifndef UIRENDERER_WINDOW_RENDERER_H
#define UIRENDERER_WINDOW_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>
#include <fontRenderer.h>
#include <settings.h>
#include <ui.h>
#include <blockRenderer.h>

#include <vector>
#include <unordered_map>


class UIRenderer
{
public:
    UIRenderer(Settings& settings, BlockRenderer& block_renderer);
    void render(UI* p_ui);

    void render_element(UIElement* p_ele, double off_x, double off_y);
    void render_frame(UIFrame* p_ele, double off_x, double off_y);
    void render_invis(UIInvis* p_ele, double off_x, double off_y);
    void render_scrollframe(UIScrollFrame* p_ele, double off_x, double off_y);
    void render_slider(UISlider* p_ele, double off_x, double off_y);
    void render_y_slider(UIYSlider* p_ele, double off_x, double off_y);
    void render_button(UIButton* p_ele, double off_x, double off_y);
    void render_blockitem(UIBlockItem* p_ele, double off_x, double off_y);
    void render_pickedblock(UIPickedBlock* p_ele, double off_x, double off_y);
    
private:
    Settings& settings;
    Shaderprogram ui_shaderprogram;
    Shaderprogram ui_tex_shaderprogram;
    unsigned int vao_quad;
    unsigned int vbo_quad;
    FontDrawer fontDrawer;
    void createShaders();
    void create_quad_vao_vbo();
    BlockRenderer& block_renderer;
};




#endif // UIRENDERER_WINDOW_RENDERER_H



