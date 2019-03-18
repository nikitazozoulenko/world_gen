#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "displaywindow.h"

class InputHandler
{
public:
    InputHandler(Displaywindow* ptr_displaywindow, Camera* ptr_camera);
    void processInput(float delta_time);
    void setCallbacks();
protected:
private:
    Displaywindow* p_displaywindow;
    Camera* p_camera;
    //TODO GameWorld* gameworld;

    float mouse_x;
    float mouse_y;
    bool firstmouse = true;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

#endif // INPUTHANDLER_H