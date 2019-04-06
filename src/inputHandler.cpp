#include "../include/inputHandler.h"

#include <iostream>
#include <cmath>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


InputHandler::InputHandler(Displaywindow* p_displaywindow, GameWorld* p_game_world)
{
    this->p_displaywindow = p_displaywindow;
    this->p_game_world = p_game_world;

    setCallbacks();

    //other options
    glfwSetInputMode(p_displaywindow->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor
}


void InputHandler::updateDeltaTime()
{
    float currentFrame = glfwGetTime();
    delta_time = currentFrame - last_frame_time;
    last_frame_time = currentFrame;
    std::cout << 1/delta_time << std::endl;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void InputHandler::processKeyboardInput()
{   
    GLFWwindow* window = p_displaywindow->window;
    //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        p_game_world->player.camera.ProcessKeyboard(Camera::DOWN, delta_time);
}


void InputHandler::setCallbacks()
{   
    GLFWwindow* window = p_displaywindow->window;
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void InputHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void InputHandler::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputHandler* my_input_handler = (InputHandler*)glfwGetWindowUserPointer(window);
    my_input_handler->p_game_world->player.camera.ProcessMouseScroll(yoffset);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void InputHandler::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   
    InputHandler* my_input_handler = (InputHandler*)glfwGetWindowUserPointer(window);
    if (my_input_handler->firstmouse)
    {
        my_input_handler->mouse_x = xpos;
        my_input_handler->mouse_y = ypos;
        my_input_handler->firstmouse = false;
    }

    float xoffset = xpos - my_input_handler->mouse_x;
    float yoffset = my_input_handler->mouse_y - ypos; // reversed since y-coordinates go from bottom to top

    my_input_handler->mouse_x = xpos;
    my_input_handler->mouse_y = ypos;

    my_input_handler->p_game_world->player.camera.ProcessMouseMovement(xoffset, yoffset);
}


void InputHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    InputHandler* inp = (InputHandler*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if(inp->p_game_world->block_is_targeted)
        {
            glm::vec3 pos = inp->p_game_world->target_pos;
            inp->p_game_world->changeBlock(pos.x, pos.y, pos.z, 0);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        if(inp->p_game_world->block_is_targeted)
        {
            glm::vec3 pos = inp->p_game_world->before_pos;
            inp->p_game_world->changeBlock(pos.x, pos.y, pos.z, 3);
        }
    }
}