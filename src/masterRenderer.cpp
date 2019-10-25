#include "../include/masterRenderer.h"

#include <iostream>

MasterRenderer::MasterRenderer(GLFWwindow* window, Settings& settings) :
    window(window),
    settings(settings),
    block_renderer(BlockRenderer(settings)),
    ui_renderer(UIRenderer())
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
}


void MasterRenderer::render(World& world, Camera& camera)
{
    //once every frame
    glClearColor(135/255.0f, 206/255.0f, 235/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    block_renderer.render(world.chunk_map, camera);
    glfwSwapBuffers(window);
}


void MasterRenderer::render_menu(Camera& camera)
{
    glClearColor(123/255.0f, 45/255.0f, 67/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ui_renderer.render();
    glfwSwapBuffers(window);
}