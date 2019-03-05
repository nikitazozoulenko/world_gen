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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    Displaywindow displaywindow = Displaywindow(SCR_WIDTH, SCR_HEIGHT, "My Window");
    Camera camera = Camera(glm::vec3(4,0,0));
    InputHandler input_handler = InputHandler(&displaywindow, &camera);

    //Shader stuff init
    Shaderprogram cube_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/block.vs", "/home/nikita/Code/world_gen/src/shaders/block.fs");
    Shaderprogram lamp_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/lamp.vs", "/home/nikita/Code/world_gen/src/shaders/lamp.fs");

    std::unordered_map<unsigned int, BlockModel> block_type_map = {{1, BlockModel()}};
    BlockModel block = BlockModel();



    //model

    Model ourModel("/home/nikita/Code/world_gen/resources/nanosuit/nanosuit.obj");
    Shaderprogram obj_shaderprogram = Shaderprogram("/home/nikita/Code/world_gen/src/shaders/object.vs", "/home/nikita/Code/world_gen/src/shaders/object.fs");





    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // shader configuration
    // --------------------
    cube_shaderprogram.use();
    cube_shaderprogram.setUniformInt("material.diffuse", 0);
    cube_shaderprogram.setUniformInt("material.specular", 1);


    // be sure to activate shader when setting uniforms/drawing objects
        cube_shaderprogram.setUniformFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        cube_shaderprogram.setUniformVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cube_shaderprogram.setUniformVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cube_shaderprogram.setUniformVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        cube_shaderprogram.setUniformVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cube_shaderprogram.setUniformVec3("pointLights[0].position", pointLightPositions[0]);
        cube_shaderprogram.setUniformVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cube_shaderprogram.setUniformVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shaderprogram.setUniformVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[0].constant", 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[0].linear", 0.09);
        cube_shaderprogram.setUniformFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        cube_shaderprogram.setUniformVec3("pointLights[1].position", pointLightPositions[1]);
        cube_shaderprogram.setUniformVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cube_shaderprogram.setUniformVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shaderprogram.setUniformVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[1].constant", 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[1].linear", 0.09);
        cube_shaderprogram.setUniformFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        cube_shaderprogram.setUniformVec3("pointLights[2].position", pointLightPositions[2]);
        cube_shaderprogram.setUniformVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cube_shaderprogram.setUniformVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shaderprogram.setUniformVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[2].constant", 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[2].linear", 0.09);
        cube_shaderprogram.setUniformFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        cube_shaderprogram.setUniformVec3("pointLights[3].position", pointLightPositions[3]);
        cube_shaderprogram.setUniformVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cube_shaderprogram.setUniformVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shaderprogram.setUniformVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[3].constant", 1.0f);
        cube_shaderprogram.setUniformFloat("pointLights[3].linear", 0.09);
        cube_shaderprogram.setUniformFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        cube_shaderprogram.setUniformVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cube_shaderprogram.setUniformVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cube_shaderprogram.setUniformFloat("spotLight.constant", 1.0f);
        cube_shaderprogram.setUniformFloat("spotLight.linear", 0.09);
        cube_shaderprogram.setUniformFloat("spotLight.quadratic", 0.032);
        cube_shaderprogram.setUniformFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
        cube_shaderprogram.setUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.0f)));    

        cube_shaderprogram.setUniformVec3("spotLight.position", camera.position);
        cube_shaderprogram.setUniformVec3("spotLight.direction", camera.front); 



    //can have this anywhere in the code, MASTERRENDERER?
    glEnable(GL_DEPTH_TEST);

    // render loop
    // -----------
    float delta_time = 0;
    float last_frame_time = 0;
    while (!glfwWindowShouldClose(displaywindow.window))
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame_time;
        last_frame_time = currentFrame;

        // input
        // -----
        input_handler.processInput(delta_time);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //activate shader
        cube_shaderprogram.use();
        cube_shaderprogram.setUniformVec3("viewPos", camera.position);
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cube_shaderprogram.setUniformMat4("projection", projection);
        cube_shaderprogram.setUniformMat4("view", view);

        //render container
        for (int i=0; i<10; i++)
        {
            for (int j=0; j<10; j++)
            {
                glm::vec3 position = glm::vec3(i,-2,j);
                block.draw(cube_shaderprogram, position);
            }
        }

        //draw lamps
        lamp_shaderprogram.use();
        lamp_shaderprogram.setUniformMat4("projection", projection);
        lamp_shaderprogram.setUniformMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lamp_shaderprogram.setUniformMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }






        //object
        obj_shaderprogram.use();
        obj_shaderprogram.setUniformMat4("projection", projection);
        obj_shaderprogram.setUniformMat4("view", view);
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        obj_shaderprogram.setUniformMat4("model", model);
        ourModel.draw(obj_shaderprogram);








        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(displaywindow.window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}







