#include "../include/displaywindow.h"
#include <iostream>



Displaywindow::Displaywindow(const unsigned int& width, const unsigned int& height, const char* title)
{
    Init(width, height, title);
}


void Displaywindow::Init(const unsigned int& width, const unsigned int& height, const char* title)
{
    // glfw: initialize and configure
    // ------------------------------
    glewExperimental=true; //needed for core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
}