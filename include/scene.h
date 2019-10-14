#ifndef SCENE_LIKE_AN_OPTIONS_MENU_H
#define SCENE_LIKE_AN_OPTIONS_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>
#include <uiElement.h>
#include <masterRenderer.h>
#include <inputScheme.h>

class Scene 
{
public:
    Scene(GLFWwindow* window);
    void render(MasterRenderer& masterRenderer);

    InputScheme input_scheme;
    Camera camera;
protected:
private:
    //std::vector<UIElement> ui_elements;
};

#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
