#ifndef MY_SKYBOXRENDERER_H
#define MY_SKYBOXRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>
#include <camera.h>
#include <settings.h>


class SkyboxRenderer
{
public:
    SkyboxRenderer(Settings& settings);
    void render(Camera& camera);
    
private:
    Settings& settings;
    Shaderprogram skybox_shaderprogram;
    unsigned int texture;
    unsigned int vao;
    unsigned int vbo;

    void createShaders();
    void createVAOVBOtex();

    glm::mat4 getProjViewMatrix(Camera& camera);
    void setProjViewMatrix(glm::mat4& projView);
};
#endif // MY_SKYBOXRENDERER_H