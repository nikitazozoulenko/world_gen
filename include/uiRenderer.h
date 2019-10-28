#ifndef UIRENDERER_H
#define UIRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>
#include <fontRenderer.h>
#include <uiWindow.h>

#include <vector>


class UIRenderer
{
public:
    UIRenderer();
    void render(std::vector<UIWindow>& ui_windows);
    void createUI();
    
private:
    Shaderprogram ui_shaderprogram;
    std::vector<UIWindow> ui_windows;
    FontDrawer fontDrawer;
    void createShaders();
};




#endif // UIRENDERER_H



