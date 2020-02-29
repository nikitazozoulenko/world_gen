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
protected:
private:
};


class MainMenu: public Scene
{
public:
    MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer);
    void render();
    void scene_logic(float delta_time);
    void createUIWindow(glm::vec2 coords, float width, float height, glm::vec3 color);
    void removeUIWindow(UIWindow* p_ui_window);
protected:
private:
    std::vector<UIWindow*> ui_windows;

    void createUI();
};


#endif // SCENE_LIKE_AN_OPTIONS_MENU_H
