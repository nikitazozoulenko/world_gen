#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <settings.h>

class Displaywindow
{
public:
    Displaywindow(Settings& settings);
    void loadGLFW();
    void loadGlad();
    void createWindow(const unsigned int& width, const unsigned int& height, const char* title);
    GLFWwindow* window;
protected:
private:
};

#endif // DISPLAYWINDOW_H