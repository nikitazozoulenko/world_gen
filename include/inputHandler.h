#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "displaywindow.h"
#include "gameWorld.h"

class InputHandler
{
public:
    InputHandler(Displaywindow* p_displaywindow, GameWorld* p_game_world);
    void processKeyboardInput();
    void setCallbacks();
    void updateDeltaTime();

protected:
private:
    Displaywindow* p_displaywindow;
    GameWorld* p_game_world;

    float mouse_x;
    float mouse_y;
    bool firstmouse = true;
    float delta_time = 0;
    float last_frame_time = 0;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};

#endif // INPUTHANDLER_H