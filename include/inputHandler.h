#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "displaywindow.h"

class InputHandler
{
public:
    InputHandler(Displaywindow* p_displaywindow, Camera* p_camera);
    void processKeyboardInput();
    void setCallbacks();
    void updateDeltaTime();

private:
    Displaywindow* p_displaywindow;
    Camera* p_camera;

    float mouse_x;
    float mouse_y;
    bool firstmouse = true;
    float delta_time = 0;
    float last_frame_time = 0;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    // static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

#endif // INPUTHANDLER_H