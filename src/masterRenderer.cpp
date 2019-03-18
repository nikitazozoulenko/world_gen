#include "../include/masterRenderer.h"

#include <iostream>

MasterRenderer::MasterRenderer(GameWorld* p_game_world, Camera* p_camera) 
    : block_renderer(BlockRenderer(p_game_world, p_camera)),
      obj_renderer(ObjectRenderer(p_game_world, p_camera)), 
      p_camera(p_camera),
      p_game_world(p_game_world)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
}


void MasterRenderer::render()
{
    //once every frame
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //blocks
    block_renderer.render();

    //objects
    obj_renderer.render();
}