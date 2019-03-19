#include "../include/inputHandler.h"

#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


InputHandler::InputHandler(Displaywindow* ptr_displaywindow, Camera* ptr_camera, GameWorld* p_game_world)
{
    p_displaywindow = ptr_displaywindow;
    p_camera = ptr_camera;
    this->p_game_world = p_game_world;

    setCallbacks();

    //other options
    glfwSetInputMode(p_displaywindow->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void InputHandler::processInput(float delta_time)
{   
    GLFWwindow* window = p_displaywindow->window;
    //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        p_camera->ProcessKeyboard(Camera::DOWN, delta_time);
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
    my_input_handler->p_camera->ProcessMouseScroll(yoffset);
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

    my_input_handler->p_camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputHandler::helper_mouse_fun(float x, float y, float z, InputHandler* inp)
{
    std::cout << "pressed" << std::endl;
    std::cout << "ints: x "<<(int)x << "y "<<(int)y<< "z "<<(int)z<<std::endl;
    std::cout << "floats: x "<<x << "y "<<y<< "z "<<z<<std::endl;
    float player_x = inp->p_camera->position.x;
    float player_y = inp->p_camera->position.y;
    float player_z = inp->p_camera->position.z;
    float dir_x = inp->p_camera->front.x;
    float dir_y = inp->p_camera->front.y;
    float dir_z = inp->p_camera->front.z;
    if((x-player_x)*(x-player_x) + (y-player_y)*(y-player_y) + (z-player_z)*(z-player_z) < 3*3)
    {
        BlockInfo info = inp->p_game_world->getBlockInfo(x,y,z);
        if(info.blockID != 0)
        {
            inp->p_game_world->removeBlock(x,y,z);
        }
        else
        {
        float x_len = (int)x - x;
        float tx = std::max(x_len/dir_x, (1+x_len)/dir_x);
        float y_len = (int)y - y;
        float ty = std::max(y_len/dir_y, (1+y_len)/dir_y);
        float z_len = (int)z - z;
        float tz = std::max(z_len/dir_z, (1+z_len)/dir_z);

        std::cout << "dir x "<<dir_x << "dir y "<<dir_y<< "dir z "<<dir_z<<std::endl;
        std::cout << "tx "<<tx << "   ty "<<ty<< "   tz "<<tz<<std::endl;
        std::cout << "xlen "<<x_len << "   ylen "<<y_len<< "   zlen "<<z_len<<std::endl;
        std::cout << "x_travel "<<tx*dir_x << "   y_travel "<<ty*dir_y<< "   z_travel "<<tz*dir_z<<std::endl;
        std::cout << "tx "<<tx << "   ty "<<ty<< "   tz "<<tz<<std::endl;

        float t;
        float eps = 0.0001;
        if(tx<ty && tx<tz)
            t=tx + eps;
        else if(ty<tx && ty<tz)
            t=ty + eps;
        else
            t=tz + eps;

        std::cout << std::endl;

        helper_mouse_fun(x+t*dir_x, y+t*dir_y, z+t*dir_z, inp);
        }
    }
}


void InputHandler::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    InputHandler* inp = (InputHandler*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        float player_x = inp->p_camera->position.x;
        float player_y = inp->p_camera->position.y;
        float player_z = inp->p_camera->position.z;
        helper_mouse_fun(player_x, player_y, player_z, inp);
    }
}