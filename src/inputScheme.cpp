#include <inputScheme.h>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <misc.h>
#include <scene.h>


// ABSTRACT class INPUT SCHEME
InputScheme::InputScheme(Settings& settings, GLFWwindow* window, Camera& camera) :
    window(window),
    camera(camera),
    settings(settings)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_map_callback);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void InputScheme::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);
}


void InputScheme::key_map_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputScheme* p_input_scheme = (InputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->key_map[key]=(bool)action;
}


void InputScheme::clear_frame_input_values()
{
    key_map.clear();
    mouse_old_x = mouse_x;
    mouse_old_y = mouse_y;
    cursor_moved = false;
}

bool InputScheme::clicked(int key)
{
    bool found = false;
    auto search = key_map.find(key);
    if(search != key_map.end())
        found = (search->second == GLFW_PRESS);
    return found;
}
bool InputScheme::held(int key)
{
    bool held = false;
    auto search = key_map.find(key);
    if(search != key_map.end()){
        bool& action = search->second;
        if(action!=GLFW_PRESS && glfwGetKey(window,key)!=GLFW_PRESS)
            held = true;
    }
    return held;
}
bool InputScheme::clicked_or_held(int key)
{
    return glfwGetKey(window,key) == GLFW_PRESS;
}
bool InputScheme::released(int key)
{
    bool released = false;
    auto search = key_map.find(key);
    if(search != key_map.end())
    {
        released = (search->second == GLFW_RELEASE);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// FreeCamWorld InputScheme ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
}


void FreeCamWorldInputScheme::start_MOUSEMOVE_mode()
{
    mode=MOUSEMOVE;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//undisable cursor

    glfwSetScrollCallback(window, NULL);
    glfwSetCursorPosCallback(window, cursor_pos_callback_MOUSEMOVE);
    glfwSetMouseButtonCallback(window, mouse_click_callback_MOUSEMOVE);
}


void FreeCamWorldInputScheme::start_CAMMOVE_mode()
{   
    mode=CAMMOVE;
    firstmouse=true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor

    glfwSetScrollCallback(window, scroll_callback_CAMMOVE);
    glfwSetCursorPosCallback(window, cursor_pos_callback_CAMMOVE);
    glfwSetMouseButtonCallback(window, NULL);
}


void FreeCamWorldInputScheme::freeCamMovementInput(double delta_time)
{
    //movement
    if (clicked_or_held(GLFW_KEY_W))
        camera.ProcessKeyboard(Camera::FORWARD, delta_time);
    if (clicked_or_held(GLFW_KEY_S))
        camera.ProcessKeyboard(Camera::BACKWARD, delta_time);
    if (clicked_or_held(GLFW_KEY_A))
        camera.ProcessKeyboard(Camera::LEFT, delta_time);
    if (clicked_or_held(GLFW_KEY_D))
        camera.ProcessKeyboard(Camera::RIGHT, delta_time);
    if (clicked_or_held(GLFW_KEY_SPACE))
        camera.ProcessKeyboard(Camera::UP, delta_time);
    if (clicked_or_held(GLFW_KEY_LEFT_CONTROL))
        camera.ProcessKeyboard(Camera::DOWN, delta_time);
}


void FreeCamWorldInputScheme::processInput(double delta_time)
{   
    glfwPollEvents();

    if(mode==CAMMOVE)
        freeCamMovementInput(delta_time);

        //escape exit window
    if (clicked(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    if (clicked(GLFW_KEY_TAB)){
        if(mode==CAMMOVE){
            start_MOUSEMOVE_mode();
        }else if(mode==MOUSEMOVE){
            start_CAMMOVE_mode();
        }
    }
    if(clicked(GLFW_KEY_L)){
        change_scene=SCENE_MainMenu;
    }
    clear_frame_input_values();
}


void FreeCamWorldInputScheme::scroll_callback_CAMMOVE(GLFWwindow* window, double xoffset, double yoffset)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->camera.ProcessMouseScroll(yoffset);
}


void FreeCamWorldInputScheme::cursor_pos_callback_CAMMOVE(GLFWwindow* window, double xpos, double ypos)
{   
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    if (p_input_scheme->firstmouse)
    {
        p_input_scheme->mouse_x = xpos;
        p_input_scheme->mouse_y = ypos;
        p_input_scheme->firstmouse = false;
    }

    double xoffset = xpos - p_input_scheme->mouse_x;
    double yoffset = p_input_scheme->mouse_y - ypos; // reversed since y-coordinates go from bottom to top

    p_input_scheme->mouse_x = xpos;
    p_input_scheme->mouse_y = ypos;

    p_input_scheme->camera.ProcessMouseMovement(xoffset, yoffset);
}


void FreeCamWorldInputScheme::mouse_click_callback_MOUSEMOVE(GLFWwindow* window, int button, int action, int mods)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    p_input_scheme->mouse_state = action;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    double x =  xpos/p_input_scheme->settings.getWindowWidth();
    double y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();

    //UIWindow::uiwindow_click_callback(action, ui_windows, x, y);
}


void FreeCamWorldInputScheme::cursor_pos_callback_MOUSEMOVE(GLFWwindow* window, double xpos, double ypos)
{   //does logic for moving UIWindows
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double x =  xpos/p_input_scheme->settings.getWindowWidth();
    double y =  1.0 - ypos/p_input_scheme->settings.getWindowHeight();
    int& mouse_state = p_input_scheme->mouse_state;

    //offsets update. special
    if (p_input_scheme->firstmouse){
        p_input_scheme->mouse_x = x;
        p_input_scheme->mouse_y = y;
        p_input_scheme->firstmouse = false;
    }
    double xoffset = x - p_input_scheme->mouse_x;
    double yoffset = y - p_input_scheme->mouse_y;
    p_input_scheme->mouse_x = x;
    p_input_scheme->mouse_y = y;

    //UIWindow::uiwindow_mouse_move_callback(mouse_state, ui_windows, xoffset, yoffset, x, y);
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
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
}


void MainMenuInputScheme::remove()
{
    glfwSetKeyCallback(window, NULL);
    glfwSetCursorPosCallback(window, NULL);
    glfwSetMouseButtonCallback(window, NULL);
}


void MainMenuInputScheme::processInput(double delta_time)
{
    glfwPollEvents();

    //keys
    if (clicked(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    if (clicked(GLFW_KEY_L))
        change_scene=SCENE_FreeCamWorld;
    
    // mouse move
    if(cursor_moved){
        process_mouse_movement();
    }

    clear_frame_input_values();
}


void MainMenuInputScheme::process_mouse_movement()
{
    if(p_pressed_window){
        p_pressed_window->process_movement(mouse_x-mouse_old_x, mouse_y-mouse_old_y, mouse_x, mouse_y);
    }
}


void MainMenuInputScheme::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   //does logic for moving UIWindows
    MainMenuInputScheme* p_input_scheme = (MainMenuInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double x =  xpos/ (double) p_input_scheme->settings.getWindowWidth();
    double y =  1.0 - ypos/ (double) p_input_scheme->settings.getWindowHeight();
    int& mouse_state = p_input_scheme->mouse_state;

    p_input_scheme->mouse_x = x;
    p_input_scheme->mouse_y = y;
    p_input_scheme->cursor_moved=true;
    if (p_input_scheme->firstmouse){
        p_input_scheme->firstmouse = false;
        p_input_scheme->cursor_moved=false;
    }
}


void MainMenuInputScheme::mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    MainMenuInputScheme* p_input_scheme = (MainMenuInputScheme*)glfwGetWindowUserPointer(window);
    std::vector<UIWindow*>& ui_windows = p_input_scheme->p_scene->ui.windows;
    p_input_scheme->mouse_state = action;
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double x = p_input_scheme->mouse_old_x;
    double y = p_input_scheme->mouse_old_y;
    UIWindow::uiwindow_click_callback(action, ui_windows, x, y, &p_input_scheme->p_pressed_window);
}