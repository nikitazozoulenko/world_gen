#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <blockRenderer.h>
#include <uiRenderer.h>
#include <world.h>
#include <uiWindow.h>

#include<vector>

class MasterRenderer
{
public:
    MasterRenderer(GLFWwindow* window, Settings& settings);
    void render_freecamworld(World& world, Camera& camera);
    void render_mainmenu(Camera& camera, std::vector<UIWindow>& ui_windows);
private:
    BlockRenderer block_renderer;
    UIRenderer ui_renderer;

    GLFWwindow* window;
    Settings& settings;
};
#endif // MASTERRENDERER_H