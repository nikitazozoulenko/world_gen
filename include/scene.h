#ifndef SCENE_LIKE_AN_OPTIONS_MENU_H
#define SCENE_LIKE_AN_OPTIONS_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>
#include <uiElement.h>
#include <masterRenderer.h>
#include <inputScheme.h>
#include <settings.h>

class Scene 
{
public:
    Scene(Settings* p_settings, GLFWwindow* window, InputScheme* p_input_scheme);
    ~Scene();
    virtual void render(MasterRenderer& masterRenderer) = 0;
    InputScheme* p_input_scheme;
    Camera camera;
protected:
    Settings* p_settings;
private:
    //std::vector<UIElement> ui_elements;
};


class FreeCamWorld: public Scene
{
public:
    FreeCamWorld(Settings* p_settings, GLFWwindow* window);
    void render(MasterRenderer& masterRenderer);
    World world;
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(GLFWwindow* window);
    void render(MasterRenderer& masterRenderer);
protected:
private:
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
