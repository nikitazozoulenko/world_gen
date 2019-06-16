#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include standard headers
#include <iostream>
#include <unordered_map>

#include "../include/displaywindow.h"
#include "../include/shaderprogram.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/inputHandler.h"
#include "../include/misc.h"
#include "../include/model.h"
#include "../include/block.h"
#include "../include/gameWorld.h"
#include "../include/masterRenderer.h"
#include "../include/worldGenerator.h"


int main()
{
    Displaywindow displaywindow = Displaywindow(1280, 720, "My Window"); //note, have to change SCRWIDTH SCRHEIGHT in multiple places (masterrenderer proj matrix)
    std::cout << "test1" << std::endl;

    GameWorld game_world;
    std::cout << "test2" << std::endl;

    InputHandler input_handler = InputHandler(&displaywindow, &game_world);
    std::cout << "test3" << std::endl;

    MasterRenderer renderer = MasterRenderer(&game_world);
    std::cout << "test4" << std::endl;

    // render loop
    while (!glfwWindowShouldClose(displaywindow.window))
    {
        // per-frame time logic
        input_handler.updateDeltaTime();
        game_world.update();
        input_handler.processKeyboardInput();

        print_vec3("pos", game_world.player.pos);
        
        //render
        renderer.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(displaywindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







