#ifndef UIWINDOW_MOVABLE_H
#define UIWINDOW_MOVABLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <shaderprogram.h>

class UIRenderer;

class UISlider
{
public:
    UISlider(double min, double max, double width, double height, double tick_width=0.5, double line_height=0.5);

    double min;
    double max;
    double value;
    bool held_down; 

    // all from [0,1] w.r.t. the size of the box its in.
    double width;
    double height;
    double tick_width;  //wrt to width
    double line_height; //wrt to height

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

    //last time the window was clicked, used to know which is top most window
    double time_last_press;
    bool held_down;

    static UIWindow* find_uiwindow_on_cursor(std::vector<UIWindow*>& ui_windows, float& x, float& y);
    static void uiwindow_mouse_move_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& xoffset, float& yoffset, float& x, float& y);
    static void uiwindow_click_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& x,float& y);

private:
    void setup();

    UISlider slider;

};





#endif // UIWINDOW_MOVABLE_H