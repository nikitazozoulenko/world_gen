#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <blockRenderer.h>
#include <uiRenderer.h>

#include<vector>

class FreeCamWorld;
class MainMenu;
class MasterRenderer
{
public:
    MasterRenderer(GLFWwindow* window, Settings& settings);
    void render_freecamworld(FreeCamWorld* p_scene);
    void render_mainmenu(MainMenu* p_scene);

    //TODO: remove from public, should be private
    GLFWwindow* window;

    BlockRenderer block_renderer;
    UIRenderer ui_renderer;
private:

    Settings& settings;
};
#endif // MASTERRENDERER_H