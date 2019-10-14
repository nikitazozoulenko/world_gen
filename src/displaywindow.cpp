#include "../include/displaywindow.h"
#include <iostream>



Displaywindow::Displaywindow(Settings& settings)
{
    loadGLFW();
    glm::vec2 dimensions = settings.getContextCreationDimensions();
    std::string window_title = settings.getWindowTitle();
    createWindow(dimensions.x, dimensions.y, window_title.c_str());
    loadGlad();
}


void Displaywindow::loadGLFW()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


void Displaywindow::loadGlad()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }    
}

void Displaywindow::createWindow(const unsigned int& width, const unsigned int& height, const char* title)
{
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
}