#ifndef INPUT_SCHEME_H
#define INPUT_SCHEME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <camera.h>
#include <vector>
#include <unordered_map>
#include <string>

class InputScheme
{
public:
    InputScheme(Settings& settings, GLFWwindow* window, Camera& camera);
    virtual void processInput(double delta_time) = 0;
    virtual void init() = 0;
    virtual void remove() = 0;

protected:
    GLFWwindow* window;
    Camera& camera;
    Settings& settings;

    double mouse_old_x=0;
    double mouse_old_y=0;
    double mouse_x=0;
    double mouse_y=0;
    bool cursor_moved = false;
    bool firstmouse = true;

    //only contains info if clicked this frame. has to be cleared every frame
    std::unordered_map<int, bool> key_map;

    bool clicked(int key);
    bool held(int key);
    bool released(int key);

    void clear_frame_input();

    void init_shared_callbacks();
    static void key_map_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
private:
};


class FreeCamWorld;
class FreeCamWorldInputScheme : public InputScheme
{
public:
    FreeCamWorldInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, FreeCamWorld* p_scene);
    void processInput(double delta_time);
    void init();
    void remove();

    enum{CAMMOVE, MOUSEMOVE};
    int mode;

private:
    FreeCamWorld* p_scene;

    void start_CAMMOVE_mode();
    void start_MOUSEMOVE_mode();

    void freeCamMovementInput(double delta_time);
    void mouseMovementInput(double delta_time);

    static void scroll_callback_CAMMOVE(GLFWwindow* window, double xoffset, double yoffset);
};



class MainMenu;
class MainMenuInputScheme : public InputScheme
{
public:
    MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, MainMenu* p_scene);
    void processInput(double delta_time);
    void init();
    void remove();
private:

    MainMenu* p_scene;
};



class Editor;
class EditorInputScheme : public InputScheme
{
public:
    EditorInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, Editor* p_scene);
    void processInput(double delta_time);
    void init();
    void remove();
private:

    Editor* p_scene;
};




#endif // INPUT_SCHEME_H