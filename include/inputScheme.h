#ifndef INPUT_SCHEME_H
#define INPUT_SCHEME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <camera.h>

class InputScheme
{
public:
    InputScheme(GLFWwindow* window, Camera* p_camera);
    virtual void processKeyboardInput(float delta_time) = 0;
    virtual void init() = 0;
protected:
    GLFWwindow* window;
    Camera* p_camera;
private:
};


class FreeCamWorldInputScheme : public InputScheme
{
public:
    FreeCamWorldInputScheme(GLFWwindow* window, Camera* p_camera);
    void processKeyboardInput(float delta_time);
    void init();
private:
    float mouse_x;
    float mouse_y;
    bool firstmouse = true;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif // INPUT_SCHEME_H