#ifndef UIRENDERER_H
#define UIRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderprogram.h"
#include "gameWorld.h"
#include "camera.h"

class UIQuad
{
public:
    UIQuad(float x0, float x1, float y0, float y1, std::string filename);
    void draw(Shaderprogram& shaderprogram);
private:
    unsigned int texture;
    unsigned int VAO;
    unsigned int VBO;
    void setup(float x0, float x1, float y0, float y1, std::string filename);
};


class UIRenderer
{
public:
    Shaderprogram ui_shaderprogram;

    UIRenderer(GameWorld* p_game_world);
    void render();
    void createUI();
    
private:
    GameWorld* p_game_world;
    std::vector<UIQuad> quads;

    void createShaders();
};




#endif // UIRENDERER_H






