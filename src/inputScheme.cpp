#include <inputScheme.h>
#include <iostream>
#include <cmath>

#include <misc.h>

///////// ABSTRACT INPUT SCHEME //////////////////////////////////////////////////////////////////////////////////
InputScheme::InputScheme(Settings& settings, GLFWwindow* window, Camera& camera) :
    window(window),
    camera(camera),
    settings(settings)
{

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void InputScheme::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// FreeCamWorld InputScheme //////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FreeCamWorldInputScheme::FreeCamWorldInputScheme(Settings& settings, GLFWwindow* window, Camera& camera) :
    InputScheme(settings, window, camera)
{

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
    glfwSetKeyCallback(window, change_scene_key_callback);
}


void FreeCamWorldInputScheme::remove()
{
    glfwSetWindowUserPointer(window, NULL);
    glfwSetScrollCallback(window, NULL);
    glfwSetCursorPosCallback(window, NULL);
    glfwSetKeyCallback(window, NULL);
}


void freeCamMovementInput(GLFWwindow* window, float delta_time, Camera& camera)
{
    //movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::DOWN, delta_time);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void FreeCamWorldInputScheme::processKeyboardInput(float delta_time)
{   
    glfwPollEvents();

    //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    freeCamMovementInput(window, delta_time, camera);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void FreeCamWorldInputScheme::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    FreeCamWorldInputScheme* my_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    my_input_scheme->camera.ProcessMouseScroll(yoffset);
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

    my_input_scheme->camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: TODO
void FreeCamWorldInputScheme::change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    FreeCamWorldInputScheme* my_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        my_input_scheme->change_scene=SCENE_MainMenu;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu InputScheme //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainMenuInputScheme::MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera) :
    InputScheme(settings, window, camera)
{

}


void MainMenuInputScheme::init()
{
    //set user defined pointer for mouse callbacks
    glfwSetWindowUserPointer(window, this);

    //other callbacks
    glfwSetKeyCallback(window, change_scene_key_callback);
}


void MainMenuInputScheme::remove()
{
    glfwSetKeyCallback(window, NULL);
}


void MainMenuInputScheme::processKeyboardInput(float delta_time)
{
    glfwPollEvents();

    //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: TODO
void MainMenuInputScheme::change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    FreeCamWorldInputScheme* my_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        my_input_scheme->change_scene=SCENE_FreeCamWorld;
}