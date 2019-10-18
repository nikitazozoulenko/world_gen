#include <inputScheme.h>

#include <iostream>
#include <cmath>

///////// ABSTRACT INPUT SCHEME //////////////////////////////////////////////////////////////////////////////////
InputScheme::InputScheme(GLFWwindow* window, Camera* p_camera) :
    window(window),
    p_camera(p_camera)
{

}


///////// FreeCamWorld InputScheme //////////////////////////////////////////////////////////////////////////////////
FreeCamWorldInputScheme::FreeCamWorldInputScheme(GLFWwindow* window, Camera* p_camera) :
    InputScheme(window, p_camera)
{

}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void FreeCamWorldInputScheme::processKeyboardInput(float delta_time)
{   
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


void FreeCamWorldInputScheme::init()
{   
    //disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor

    //set user defined pointer for mouse callbacks
    glfwSetWindowUserPointer(window, this);

    //other callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void FreeCamWorldInputScheme::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void FreeCamWorldInputScheme::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    FreeCamWorldInputScheme* my_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    my_input_scheme->p_camera->ProcessMouseScroll(yoffset);
}


// glfw: whenever the mouse moves, this callback is called
void FreeCamWorldInputScheme::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   
    FreeCamWorldInputScheme* my_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (my_input_scheme->firstmouse)
    {
        my_input_scheme->mouse_x = xpos;
        my_input_scheme->mouse_y = ypos;
        my_input_scheme->firstmouse = false;
    }

    float xoffset = xpos - my_input_scheme->mouse_x;
    float yoffset = my_input_scheme->mouse_y - ypos; // reversed since y-coordinates go from bottom to top

    my_input_scheme->mouse_x = xpos;
    my_input_scheme->mouse_y = ypos;

    my_input_scheme->p_camera->ProcessMouseMovement(xoffset, yoffset);
}