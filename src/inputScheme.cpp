#include <inputScheme.h>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <misc.h>
#include <scene.h>


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

FreeCamWorldInputScheme::FreeCamWorldInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, FreeCamWorld* p_scene) :
    InputScheme(settings, window, camera),
    p_scene(p_scene),
    mode(CAMMOVE)
{

}


void FreeCamWorldInputScheme::init()
{   
    //set user defined pointer for mouse callbacks
    glfwSetWindowUserPointer(window, this);

    //other callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(mode==CAMMOVE)
        start_CAMMOVE_mode();
    else if(mode==MOUSEMOVE)
        start_MOUSEMOVE_mode();
}


void FreeCamWorldInputScheme::remove()
{
    glfwSetWindowUserPointer(window, NULL);
    glfwSetScrollCallback(window, NULL);
    glfwSetCursorPosCallback(window, NULL);
    glfwSetKeyCallback(window, NULL);
}


void FreeCamWorldInputScheme::start_MOUSEMOVE_mode()
{
    mode=MOUSEMOVE;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//undisable cursor

    glfwSetScrollCallback(window, NULL);
    glfwSetCursorPosCallback(window, cursor_pos_callback_MOUSEMOVE);
    glfwSetKeyCallback(window, change_scene_key_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback_MOUSEMOVE);
}


void FreeCamWorldInputScheme::start_CAMMOVE_mode()
{   
    mode=CAMMOVE;
    firstmouse=true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor

    glfwSetScrollCallback(window, scroll_callback_CAMMOVE);
    glfwSetCursorPosCallback(window, cursor_pos_callback_CAMMOVE);
    glfwSetKeyCallback(window, change_scene_key_callback);
    glfwSetMouseButtonCallback(window, NULL);
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

    if(mode==CAMMOVE)
        freeCamMovementInput(window, delta_time, camera);

        //escape exit window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
        if(mode==CAMMOVE){
            start_MOUSEMOVE_mode();
        }else if(mode==MOUSEMOVE){
            start_CAMMOVE_mode();
        }
    }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void FreeCamWorldInputScheme::scroll_callback_CAMMOVE(GLFWwindow* window, double xoffset, double yoffset)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->camera.ProcessMouseScroll(yoffset);
}


// glfw: whenever the mouse moves, this callback is called
void FreeCamWorldInputScheme::cursor_pos_callback_CAMMOVE(GLFWwindow* window, double xpos, double ypos)
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


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void FreeCamWorldInputScheme::mouse_click_callback_MOUSEMOVE(GLFWwindow* window, int button, int action, int mods)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    p_input_scheme->mouse_state = action;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float x =  xpos/p_input_scheme->settings.getWindowWidth();
    float y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();

    UIWindow::uiwindow_click_callback(action, ui_windows, x, y);
}


// glfw: whenever the mouse moves, this callback is called
void FreeCamWorldInputScheme::cursor_pos_callback_MOUSEMOVE(GLFWwindow* window, double xpos, double ypos)
{   //does logic for moving UIWindows
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu InputScheme //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MainMenuInputScheme::MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, MainMenu* p_scene) :
    InputScheme(settings, window, camera),
    p_scene(p_scene),
    mouse_state(0)
{

}


void MainMenuInputScheme::init()
{   
    //cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//undisable cursor

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
    glfwSetMouseButtonCallback(window, NULL);
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
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
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
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    p_input_scheme->mouse_state = action;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float x =  xpos/p_input_scheme->settings.getWindowWidth();
    float y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();

    UIWindow::uiwindow_click_callback(action, ui_windows, x, y);
}