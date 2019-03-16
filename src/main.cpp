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
    Displaywindow displaywindow = Displaywindow(800, 600, "My Window"); //note, have to change SCRWIDTH SCRHEIGHT in multiple places (masterrenderer proj matrix)
    Camera camera = Camera(glm::vec3(4,0,0));
    InputHandler input_handler = InputHandler(&displaywindow, &camera);

    WorldGenerator world_gen = WorldGenerator(123456);
    GameWorld game_world = GameWorld(&world_gen);
    MasterRenderer renderer = MasterRenderer(&game_world, &camera);

    // render loop
    float delta_time = 0;
    float last_frame_time = 0;
    while (!glfwWindowShouldClose(displaywindow.window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame_time;

        // input
        input_handler.processInput(delta_time);

        std::cout << (int) camera.position.x << " " << (int) camera.position.y << " " << (int) camera.position.z << std::endl;
        
        //render
        renderer.render();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(displaywindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}







