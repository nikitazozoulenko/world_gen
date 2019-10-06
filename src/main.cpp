#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>

#include "../include/displaywindow.h"
#include "../include/masterRenderer.h"
#include "../include/camera.h"
#include "../include/inputHandler.h"
#include "../include/misc.h"


int main()
{
    Displaywindow displaywindow = Displaywindow(1280, 720, "Marching Cubes"); //note, have to change SCRWIDTH SCRHEIGHT in multiple places (masterrenderer proj matrix)
    Camera camera;
    InputHandler input_handler = InputHandler(&displaywindow, &camera);
    MasterRenderer renderer = MasterRenderer(&camera);

    // render loop
    while (!glfwWindowShouldClose(displaywindow.window))
    {
        // // per-frame time logic
        input_handler.updateDeltaTime();
        input_handler.processKeyboardInput();
        print_vec3("pos", camera.pos);
        
        // //render
        renderer.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(displaywindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







