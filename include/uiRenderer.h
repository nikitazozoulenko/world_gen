#ifndef UIRENDERER_WINDOW_RENDERER_H
#define UIRENDERER_WINDOW_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>
#include <fontRenderer.h>

#include <vector>
#include <unordered_map>


class UIWindow;
class UISlider;

class UIRenderer
{
public:
    UIRenderer();
    void render(std::vector<UIWindow*>& ui_windows);
    void render_window(UIWindow* p_ui_window);
    void render_window_slider(UISlider& slider, UIWindow* p_ui_window);

    // void add_ui_window(UIWindow* p_ui_window);
    // void remove_ui_window(UIWindow* p_ui_window);
    
private:
    Shaderprogram ui_shaderprogram;
    unsigned int vao_quad;
    unsigned int vbo_quad;
    // std::unordered_map<UIWindow*, unsigned int> VAOS;
    // std::unordered_map<UIWindow*, unsigned int> VBOS;
    FontDrawer fontDrawer;
    void createShaders();
    void create_quad_vao_vbo();
};




#endif // UIRENDERER_WINDOW_RENDERER_H



