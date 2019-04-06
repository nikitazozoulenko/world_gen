#include "../include/masterRenderer.h"

#include <iostream>

MasterRenderer::MasterRenderer(GameWorld* p_game_world) 
    : block_renderer(BlockRenderer(p_game_world)),
      obj_renderer(ObjectRenderer(p_game_world)),
      ui_renderer(UIRenderer(p_game_world)),
      p_game_world(p_game_world)
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
}


void MasterRenderer::render()
{
    //once every frame
    glClearColor(135/255.0f, 206/255.0f, 235/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //blocks
    block_renderer.render();

    //objects
    obj_renderer.render();

    //finally ui
    ui_renderer.render();
}