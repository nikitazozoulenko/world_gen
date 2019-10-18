#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <blockRenderer.h>
#include <uiRenderer.h>
#include <world.h>

class MasterRenderer
{
public:
    MasterRenderer(GLFWwindow* window, Settings* p_settings);
    void render(World& world, Camera& camera);
private:
    BlockRenderer block_renderer;
    UIRenderer ui_renderer;

    GLFWwindow* window;
    Settings* p_settings;
};
#endif // MASTERRENDERER_H