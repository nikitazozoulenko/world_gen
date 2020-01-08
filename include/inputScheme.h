#ifndef INPUT_SCHEME_H
#define INPUT_SCHEME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <camera.h>
#include <uiWindow.h>

#include <vector>

class InputScheme
{
public:
    InputScheme(Settings& settings, GLFWwindow* window, Camera& camera);
    virtual void processInput(float delta_time) = 0;
    virtual void init() = 0;
    virtual void remove() = 0;

    //use the unique identifer that each scene has
    int change_scene=0;

protected:
    GLFWwindow* window;
    Camera& camera;
    Settings& settings;

    float mouse_x;
    float mouse_y;
    bool firstmouse = true;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
private:
};


class FreeCamWorldInputScheme : public InputScheme
{
public:
    FreeCamWorldInputScheme(Settings& settings, GLFWwindow* window, Camera& camera);
    void processInput(float delta_time);
    void init();
    void remove();
private:
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


class MainMenuInputScheme : public InputScheme
{
public:
    MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, std::vector<UIWindow>& ui_windows);
    void processInput(float delta_time);
    void init();
    void remove();
private:
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    std::vector<UIWindow>& ui_windows;
    int last_mouse_state;
};




#endif // INPUT_SCHEME_H