#ifndef MY_PROGRAM_H
#define MY_PROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <settings.h>
#include <masterRenderer.h>
#include <scene.h>

class Program
{
public:
    Program();
    void run();
private:
    // Universal settings for everything
    Settings settings;

    // OpenGL initialization stuff, context + window
    void loadGLFW();
    void loadGlad();
    GLFWwindow* createWindow(Settings& settings);
    GLFWwindow* window;

    // Renderer
    MasterRenderer renderer;

    // Current Scene, game world may be included in scene
    Scene scene;
};

#endif // MY_PROGRAM_H