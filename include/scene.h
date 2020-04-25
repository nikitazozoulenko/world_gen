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
#include <uiRenderer.h>
#include <ui.h>


class Scene 
{
public:
    Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme, MasterRenderer& masterRenderer);
    ~Scene();
    virtual void render() = 0;
    virtual void scene_logic(float delta_time) = 0;
    InputScheme* p_input_scheme;
    MasterRenderer& masterRenderer;
    Camera camera;
protected:
    Settings& settings;
private:
};


class FreeCamWorld: public Scene
{
public:
    FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    void render();
    void scene_logic(float delta_time);
    World world;
    UI_FreeCamWorld ui;

    void slider_x_fun();
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    void render();
    void scene_logic(float delta_time);
    void test();
    UI_MainMenu ui;
protected:
private:
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
