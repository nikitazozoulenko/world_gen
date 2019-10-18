#ifndef UIRENDERER_H
#define UIRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>
#include <camera.h>

#include <vector>
#include <map>

struct Character 
{
    unsigned int texID;    // ID handle of the glyph texture
    glm::ivec2 size;       // Size of glyph
    glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;  // Offset to advance to next glyph
};


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


class FontDrawer
{
public:
    FontDrawer(std::string truetype_file_path);
    void draw(std::string text, float x, float y, float scale, glm::vec3 color);
private:
    std::map<char, Character> character_map;
    unsigned int VAO;
    unsigned int VBO;
    Shaderprogram char_shaderprogram;

    void makeCharacterTextureMap(std::string truetype_file_path);
    void setupVAOVBO();
    void setupCharShader();
};


class UIRenderer
{
public:
    UIRenderer();
    void render();
    void createUI();
    
private:
    Shaderprogram ui_shaderprogram;
    std::vector<UIQuad> quads;
    FontDrawer fontDrawer;
    void createShaders();
};




#endif // UIRENDERER_H



