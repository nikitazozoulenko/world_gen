#ifndef UIWINDOW_MOVABLE_H
#define UIWINDOW_MOVABLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>

#include <shaderprogram.h>

class UISlider
{
public:
    UISlider(float min, float max, glm::vec2 coords, float width, float height, float tick_width, float line_height, std::function<void()>& fun);

    float min;
    float max;
    float value;
    bool held_down;
    std::function<void()> fun;

    // all from [0,1] w.r.t. the size of the box its in.
    glm::vec2 coords;
    float width;
    float height;
    float tick_width;  //wrt to width
    float line_height; //wrt to height

    //will be run when mouse action is in the "slider space"
    static void mouse_move_callback(int& mouse_state, float& xoffset, float& yoffset, float& x, float& y);
    static void click_callback(int& mouse_state, float& x,float& y);
private:
};



class UIWindow
{
public:
    UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color);

    void draw(Shaderprogram& shaderprogram);

    //(x0, y0, x0+width, y0+height) normalized [0,1].
    glm::vec2 coords;
    float width;
    float height;

    //color in [0,1]^3
    glm::vec3 color;

    //ui elements
    std::vector<UISlider> sliders;
    UISlider* p_pressed_slider;

    //last time the window was clicked, used to know which is top most window
    double time_last_press;
    bool held_down;

    static UISlider* find_slider_on_cursor(UIWindow* p_ui_window, float& x, float& y);
    static UIWindow* find_uiwindow_on_cursor(std::vector<UIWindow*>& ui_windows, float& x, float& y);
    static UISlider* find_if_on_slider(UIWindow* p_ui_window, UISlider& slider, float& x, float& y);
    static UISlider* find_if_on_slider_tick(UIWindow* p_ui_window, UISlider& slider, float& x, float& y);
    static void uiwindow_mouse_move_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& xoffset, float& yoffset, float& x, float& y);
    static void uiwindow_click_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& x,float& y);

private:
    void setup();



};





#endif // UIWINDOW_MOVABLE_H