#ifndef UIWINDOW_MOVABLE_H
#define UIWINDOW_MOVABLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <shaderprogram.h>

class UIWindow
// What window should be:
//     *Create window,
//     *give window buttons, text, sliders,
//     *give layout
{
public:
    UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color);

    void draw(Shaderprogram& shaderprogram);

    unsigned int VAO;
    unsigned int VBO;

    //(x0, y0, x0+width, y0+height) normalized [0,1].
    glm::vec2 coords;
    float width;
    float height;

    //color in [0,1]^3
    glm::vec3 color;

    //last time the window was clicked, used to know which is top most window
    double time_last_press;
    bool held_down;

    static UIWindow* find_uiwindow_on_cursor(std::vector<UIWindow>& ui_windows, float& x, float& y);
    static void uiwindow_mouse_move_callback(int& mouse_state, std::vector<UIWindow>& ui_windows, float& xoffset, float& yoffset, float& x, float& y);
    static void uiwindow_click_callback(int& mouse_state, std::vector<UIWindow>& ui_windows, float& x,float& y);

private:
    void setup();
};

#endif // UIWINDOW_MOVABLE_H