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

#define SCENE_FreeCamWorld  18823
#define SCENE_MainMenu      24354
#define SCENE_ExitGame      666
#define SCENE_Editor        54321

class Scene 
{
public:
    Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme, MasterRenderer& masterRenderer, UI* p_ui);
    virtual ~Scene();
    virtual void render() = 0;
    virtual void scene_logic(double delta_time) = 0;
    virtual void end_scene();
    InputScheme* p_input_scheme;
    MasterRenderer& masterRenderer;
    Camera camera;
    UI* p_ui;

    //use the unique identifer that each scene has
    int change_scene=0;

    unsigned int frame=0;
    std::unordered_map<std::string, std::function<void(double, double)>> slider_functions;
    std::unordered_map<std::string, std::function<void()>> button_functions;
    void test_slider_fun(double val, double change);
    void slider_nothing(double val, double change);

    void change_scene_freecamworld();
    void change_scene_mainmenu();
    void change_scene_editor();
    void exit_game();
    void button_nothing();
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

    void toggle_menu();
    void toggle_inventory();
    bool is_cursor_mode();
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

protected:
private:
};


class Editor: public Scene
{
public:
    Editor(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    ~Editor();
    void render();
    void scene_logic(double delta_time);
    void end_scene();

protected:
private:
};



#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
