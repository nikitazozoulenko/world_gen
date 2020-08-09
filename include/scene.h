#ifndef SCENE_LIKE_AN_OPTIONS_MENU_H
#define SCENE_LIKE_AN_OPTIONS_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>
#include <masterRenderer.h>
#include <inputScheme.h>
#include <settings.h>
#include <uiRenderer.h>
#include <ui.h>
#include <unordered_map>
#include <functional>


class Scene 
{
public:
    Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme, MasterRenderer& masterRenderer);
    virtual ~Scene();
    virtual void render() = 0;
    virtual void scene_logic(double delta_time) = 0;
    virtual void end_scene();
    InputScheme* p_input_scheme;
    MasterRenderer& masterRenderer;
    Camera camera;
    UI ui;

    unsigned int frame=0;
    std::unordered_map<std::string, std::function<void(double, double)>> slider_functions;
    void test_slider_fun(double val, double change);
    //std::unordered_map<std::string, std::function<void(void)>> button_functions;
protected:
    Settings& settings;
private:
};


class FreeCamWorld: public Scene
{
public:
    FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    ~FreeCamWorld();
    void render();
    void scene_logic(double delta_time);
    void end_scene();
    World world;

    void slider_amp_fun(double val, double change);
    void slider_size_fun(double val, double change);
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    ~MainMenu();
    void render();
    void scene_logic(double delta_time);
    void end_scene();
    void test(double val, double change);
protected:
private:
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
