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
    init_shared_callbacks();
}



void InputScheme::init_shared_callbacks()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_map_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void InputScheme::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



void InputScheme::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{   //records cursor movement
    InputScheme* p_input_scheme = (InputScheme*)glfwGetWindowUserPointer(window);
    //normalizes [0,1] coords. y from bot to top, x from left to right
    double x = xpos;
    double y = p_input_scheme->settings.getWindowHeight() - ypos;

    p_input_scheme->mouse_x = x;
    p_input_scheme->mouse_y = y;
    p_input_scheme->cursor_moved=true;
    if (p_input_scheme->firstmouse){
        p_input_scheme->mouse_old_x = x;
        p_input_scheme->mouse_old_y = y;
        p_input_scheme->firstmouse = false;
        p_input_scheme->cursor_moved=false;
    }
}



void InputScheme::key_map_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputScheme* p_input_scheme = (InputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->key_map[key]=(bool)action;
}



void InputScheme::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    InputScheme* p_input_scheme = (InputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->key_map[button]=(bool)action;
}



void InputScheme::clear_frame_input()
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
    return released;
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
    glfwSetScrollCallback(window, NULL);
}


void FreeCamWorldInputScheme::start_MOUSEMOVE_mode()
{
    mode=MOUSEMOVE;
    firstmouse=true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//undisable cursor
    glfwSetScrollCallback(window, NULL);
}


void FreeCamWorldInputScheme::start_CAMMOVE_mode()
{   
    mode=CAMMOVE;
    firstmouse=true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable cursor
    glfwSetScrollCallback(window, scroll_callback_CAMMOVE);
}


void FreeCamWorldInputScheme::freeCamMovementInput(double delta_time)
{
    //keyboard
    if (held(GLFW_KEY_W))
        camera.ProcessKeyboard(Camera::FORWARD, delta_time);
    if (held(GLFW_KEY_S))
        camera.ProcessKeyboard(Camera::BACKWARD, delta_time);
    if (held(GLFW_KEY_A))
        camera.ProcessKeyboard(Camera::LEFT, delta_time);
    if (held(GLFW_KEY_D))
        camera.ProcessKeyboard(Camera::RIGHT, delta_time);
    if (held(GLFW_KEY_SPACE))
        camera.ProcessKeyboard(Camera::UP, delta_time);
    if (held(GLFW_KEY_LEFT_CONTROL))
        camera.ProcessKeyboard(Camera::DOWN, delta_time);

    //mouse
    if(cursor_moved)
        camera.ProcessMouseMovement(mouse_x-mouse_old_x, mouse_y-mouse_old_y);
}


void FreeCamWorldInputScheme::mouseMovementInput(double delta_time)
{
    if(clicked(GLFW_MOUSE_BUTTON_LEFT)){
        UIWindow::uiwindow_click(p_scene->ui.windows, mouse_x, mouse_y, &p_pressed_window);
    }
    if(released(GLFW_MOUSE_BUTTON_LEFT)){
        UIWindow::uiwindow_release(&p_pressed_window);
    }
    if(cursor_moved)
        if(p_pressed_window)
            p_pressed_window->process_movement(mouse_x-mouse_old_x, mouse_y-mouse_old_y, mouse_x, mouse_y);
}

void FreeCamWorldInputScheme::processInput(double delta_time)
{   
    glfwPollEvents();

    if(mode==CAMMOVE)
        freeCamMovementInput(delta_time);
    if(mode==MOUSEMOVE)
        mouseMovementInput(delta_time);

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
    clear_frame_input();
}


void FreeCamWorldInputScheme::scroll_callback_CAMMOVE(GLFWwindow* window, double xoffset, double yoffset)
{
    FreeCamWorldInputScheme* p_input_scheme = (FreeCamWorldInputScheme*)glfwGetWindowUserPointer(window);
    p_input_scheme->camera.ProcessMouseScroll(yoffset);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu InputScheme //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MainMenuInputScheme::MainMenuInputScheme(Settings& settings, GLFWwindow* window, Camera& camera, MainMenu* p_scene) :
    InputScheme(settings, window, camera),
    p_scene(p_scene)
{

}


void MainMenuInputScheme::init()
{   
    //cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//undisable cursor
    //set user defined pointer for mouse callbacks
    glfwSetWindowUserPointer(window, this);
}


void MainMenuInputScheme::remove()
{
}


void MainMenuInputScheme::processInput(double delta_time)
{
    glfwPollEvents();
    if(clicked(GLFW_MOUSE_BUTTON_LEFT))
        UIWindow::uiwindow_click(p_scene->ui.windows, mouse_x, mouse_y, &p_pressed_window);
    if(released(GLFW_MOUSE_BUTTON_LEFT))
        UIWindow::uiwindow_release(&p_pressed_window);


    //keys
    if (clicked(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
    if (clicked(GLFW_KEY_L))
        change_scene=SCENE_FreeCamWorld;
    
    // mouse move
    if(cursor_moved){
        process_mouse_movement();
    }

    clear_frame_input();
}


void MainMenuInputScheme::process_mouse_movement()
{
    if(p_pressed_window){
        p_pressed_window->process_movement(mouse_x-mouse_old_x, mouse_y-mouse_old_y, mouse_x, mouse_y);
    }
}