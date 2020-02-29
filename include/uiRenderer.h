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

class UIRenderer
{
public:
    UIRenderer();
    void render(std::vector<UIWindow*>& ui_windows);

    void add_ui_window(UIWindow* p_ui_window);
    void remove_ui_window(UIWindow* p_ui_window);
    
private:
    Shaderprogram ui_shaderprogram;
    std::unordered_map<UIWindow*, unsigned int> VAOS;
    std::unordered_map<UIWindow*, unsigned int> VBOS;
    FontDrawer fontDrawer;
    void createShaders();
};




#endif // UIRENDERER_WINDOW_RENDERER_H



