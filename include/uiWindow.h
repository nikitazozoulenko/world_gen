#ifndef UIWINDOW_MOVABLE_H
#define UIWINDOW_MOVABLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaderprogram.h>

class UIWindow
{
public:
    UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color);

    void draw(Shaderprogram& shaderprogram);

    unsigned int VAO;
    unsigned int VBO;

    //(x0, y0, x0+width, y0+height) normalized [0,1].
    glm::vec2 coords;
    float width;
    float height;

    //color in [0,1]^3
    glm::vec3 color;

    //last time the window was clicked
    double last_clicked;

private:
    void setup();
};

#endif // UIWINDOW_MOVABLE_H