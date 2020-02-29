#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <blockRenderer.h>
#include <world.h>
#include <uiWindow.h>
#include <uiRenderer.h>

#include<vector>

class MasterRenderer
{
public:
    MasterRenderer(GLFWwindow* window, Settings& settings);
    void render_freecamworld(World& world, Camera& camera);
    void render_mainmenu(std::vector<UIWindow*>& ui_windows, Camera& camera);

    //TODO: remove from public, should be private
    GLFWwindow* window;

    BlockRenderer block_renderer;
    UIRenderer ui_renderer;
private:

    Settings& settings;
};
#endif // MASTERRENDERER_H