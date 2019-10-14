#include "../include/masterRenderer.h"

#include <iostream>

MasterRenderer::MasterRenderer(GLFWwindow* window, Settings* p_settings) :
    window(window),
    p_settings(p_settings),
    block_renderer(BlockRenderer(p_settings)),
    ui_renderer(UIRenderer())
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
}


void MasterRenderer::render(Camera* p_camera)
{
    //once every frame
    glClearColor(135/255.0f, 206/255.0f, 235/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //blocks
    block_renderer.render(p_camera);
    ui_renderer.render();
    glfwSwapBuffers(window);
}