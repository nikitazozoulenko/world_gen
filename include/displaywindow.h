#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Displaywindow
{
public:
    Displaywindow(const unsigned int& width, const unsigned int& height, const char* title);
    void Init(const unsigned int& width, const unsigned int& height, const char* title);
    void ProcessInput();
    GLFWwindow* window;
protected:
private:
};

#endif // DISPLAYWINDOW_H