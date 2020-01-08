#include <inputScheme.h>
#include <iostream>
#include <cmath>
#include <algorithm>

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
void FreeCamWorldInputScheme::processInput(float delta_time)
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
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->camera.ProcessMouseScroll(yoffset);
}


// glfw: whenever the mouse moves, this callback is called
void FreeCamWorldInputScheme::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (p_input_scheme->firstmouse)
    {
        p_input_scheme->mouse_x = xpos;
        p_input_scheme->mouse_y = ypos;
        p_input_scheme->firstmouse = false;
    }

    float xoffset = xpos - p_input_scheme->mouse_x;
    float yoffset = p_input_scheme->mouse_y - ypos; // reversed since y-coordinates go from bottom to top

    p_input_scheme->mouse_x = xpos;
    p_input_scheme->mouse_y = ypos;

    p_input_scheme->camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: I required a callback to change scenes so it doesnt get used twice and change back directly
void FreeCamWorldInputScheme::change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        p_input_scheme->change_scene=SCENE_MainMenu;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu InputScheme //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MainMenuInputScheme::MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, std::vector<UIWindow>& ui_windows) :
    InputScheme(settings, window, camera),
    ui_windows(ui_windows),
    mouse_state(0)
{

}


void MainMenuInputScheme::init()
{   
    //cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//disable cursor

    //set user defined pointer for mouse callbacks
    glfwSetWindowUserPointer(window, this);

    //other callbacks
    glfwSetKeyCallback(window, change_scene_key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
}


void MainMenuInputScheme::remove()
{
    glfwSetKeyCallback(window, NULL);
    glfwSetCursorPosCallback(window, NULL);
}


void MainMenuInputScheme::processInput(float delta_time)
{
    glfwPollEvents();

    //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: I required a callback to change scenes so it doesnt get used twice and change back directly
void MainMenuInputScheme::change_scene_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        p_input_scheme->change_scene=SCENE_FreeCamWorld;
}


// glfw: whenever the mouse moves, this callback is called
void MainMenuInputScheme::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   //does logic for moving UIWindows
    MainMenuInputScheme* p_input_scheme = (MainMenuInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow>& ui_windows = p_input_scheme->ui_windows;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    float x =  xpos/p_input_scheme->settings.getWindowWidth();
    float y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();
    int& mouse_state = p_input_scheme->mouse_state;

    //offsets update. special
    if (p_input_scheme->firstmouse){
        p_input_scheme->mouse_x = x;
        p_input_scheme->mouse_y = y;
        p_input_scheme->firstmouse = false;
    }
    float xoffset = x - p_input_scheme->mouse_x;
    float yoffset = y - p_input_scheme->mouse_y;
    p_input_scheme->mouse_x = x;
    p_input_scheme->mouse_y = y;

    UIWindow::uiwindow_mouse_move_callback(mouse_state, ui_windows, xoffset, yoffset, x, y);
}


// glfw: whenever the mouse is pressed or released, this callback is called (only when state changes)
void MainMenuInputScheme::mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    MainMenuInputScheme* p_input_scheme = (MainMenuInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow>& ui_windows = p_input_scheme->ui_windows;
    p_input_scheme->mouse_state = action;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float x =  xpos/p_input_scheme->settings.getWindowWidth();
    float y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();

    UIWindow::uiwindow_click_callback(action, ui_windows, x, y);
}