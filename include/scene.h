#ifndef SCENE_LIKE_AN_OPTIONS_MENU_H
#define SCENE_LIKE_AN_OPTIONS_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>
#include <masterRenderer.h>
#include <inputScheme.h>
#include <settings.h>
#include <uiWindow.h>


class Scene 
{
public:
    Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme);
    ~Scene();
    virtual void render(MasterRenderer& masterRenderer) = 0;
    InputScheme* p_input_scheme;
    Camera camera;
protected:
    Settings& settings;
private:
};


class FreeCamWorld: public Scene
{
public:
    FreeCamWorld(Settings& settings, GLFWwindow* window);
    void render(MasterRenderer& masterRenderer);
    World world;
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(Settings& settings, GLFWwindow* window);
    void render(MasterRenderer& masterRenderer);
protected:
private:
    std::vector<UIWindow> ui_windows;

    void createUI();
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
