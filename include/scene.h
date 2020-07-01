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
    virtual void scene_logic(double delta_time) = 0;
    InputScheme* p_input_scheme;
    MasterRenderer& masterRenderer;
    Camera camera;

    unsigned int frame=0;
protected:
    Settings& settings;
private:
};


class FreeCamWorld: public Scene
{
public:
    FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    void render();
    void scene_logic(double delta_time);
    World world;
    UI_FreeCamWorld ui;

    void slider_amp_fun(double val, double change);
    void slider_size_fun(double val, double change);
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    void render();
    void scene_logic(double delta_time);
    void test(double val, double change);
    UI_MainMenu ui;
protected:
private:
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
