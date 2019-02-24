#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Include standard headers
#include <iostream>

#include "../include/displaywindow.h"
#include "../include/shaderprogram.h"
#include "../include/shader.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    Displaywindow displaywindow = Displaywindow(SCR_WIDTH, SCR_HEIGHT, "My Window");



    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader vertexShader = Shader("/home/nikita/Code/world_gen/src/shaders/vertexShader.txt", GL_VERTEX_SHADER);
    Shader fragmentShader = Shader("/home/nikita/Code/world_gen/src/shaders/fragmentShader.txt", GL_FRAGMENT_SHADER);
    Shaderprogram shaderprogram = Shaderprogram(vertexShader, fragmentShader);

    glUseProgram(shaderprogram.shaderProgramID);



    // render loop
    // -----------
    while (!glfwWindowShouldClose(displaywindow.window))
    {
        // input
        // -----
        displaywindow.ProcessInput();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(displaywindow.window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}



