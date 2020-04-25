#include <masterRenderer.h>
#include <scene.h>

#include <iostream>

MasterRenderer::MasterRenderer(GLFWwindow* window, Settings& settings) :
    window(window),
    settings(settings),
    block_renderer(BlockRenderer(settings)),
    ui_renderer(UIRenderer(settings))
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
}


void MasterRenderer::render_freecamworld(FreeCamWorld* p_scene)
{
    //once every frame
    glClearColor(135/255.0f, 206/255.0f, 235/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    block_renderer.render(p_scene->world.chunk_map, p_scene->camera);
    ui_renderer.render(p_scene->ui.windows);
    glfwSwapBuffers(window);
}


void MasterRenderer::render_mainmenu(MainMenu* p_scene)
{
    glClearColor(123/255.0f, 45/255.0f, 67/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //TODO only use new renderer not masterRenderer old ideas
    ui_renderer.render(p_scene->ui.windows);
    glfwSwapBuffers(window);
}