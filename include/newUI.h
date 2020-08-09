// #ifndef WORLDGEN_NIKITA_NEW_UI_H
// #define WORLDGEN_NIKITA_NEW_UI_H

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #include <vector>
// #include <functional>

// #include <shaderprogram.h>

// class UIWindow; //forward declare
// class UISlider
// {
// public:
//     UISlider(double min, double max, double x0, double y0, double w, double h, double tick_w, double line_h, 
//         std::function<void(double, double)>& fun);

//     double min;
//     double max;
//     double value;
//     std::function<void(double, double)> fun;
//     bool tick_held_down;

//     // sizes in pixels
//     double x0;
//     double y0;
//     double w;
//     double h;
//     double tick_w;
//     double line_h;

//     void update_value(double xoff, double win_width);
//     bool x_on_tick(double x, double win_x0);
//     bool x_on_slider(double x, double win_x0);
//     bool find_if_on_slider(UIWindow* p_ui_window, double& x, double& y);
// private:
// };



// class UIWindow
// {
// public:
//     UIWindow(double x0, double y0, double w, double h, glm::vec3 color);

//     void draw(Shaderprogram& shaderprogram);

//     //(x0, y0, x0+w, y0+h) size in pixels
//     double x0;
//     double y0;
//     double w;
//     double h;

//     //color in [0,1]^3
//     glm::vec3 color;

//     //ui elements
//     std::vector<UISlider> sliders;
//     UISlider* p_pressed_slider;

//     //last time the window was clicked, used to know which is top most window
//     double time_last_press;

//     void process_movement(double xoff, double yoff, double x, double y);

//     static UISlider* find_slider_on_cursor(UIWindow* p_ui_window, double& x, double& y);
//     static UIWindow* find_uiwindow_on_cursor(std::vector<UIWindow*>& ui_windows, double& x, double& y);
//     static void uiwindow_click(std::vector<UIWindow*>& ui_windows, double& x,double& y, UIWindow** p_pressed_window);
//     static void uiwindow_release(UIWindow** p_inputscheme_pressed_window);

// private:

// };


// class UIElement
// {
// public:
//     UIElement(double x0, double y0, double w, double h, glm::vec3 color);
//     virtual ~UIElement();

//     virtual void mouse_click(double& x, double& y);
//     virtual void mouse_release();
//     virtual void process_movement(double xoff, double yoff, double x, double y);

//     UIElement* p_parent;
//     std::vector<UIElement*> children;
//     //(x0, y0, x0+w, y0+h) size in pixels
//     double x0;
//     double y0;
//     double w;
//     double h;
//     //color in [0,1]^3
//     glm::vec3 color;
//     //other
//     double time_last_press;
// private:
// };

// class UIFrame : public UIElement
// {
// public:
//     UIFrame(double x0, double y0, double w, double h, glm::vec3 color, bool moveable);
//     bool moveable;
//     void mouse_click(double& x, double& y);
//     void mouse_release();
//     void process_movement(double xoff, double yoff, double x, double y);
// private:
// };


// class UI 
// {
// public:
//     UI();
//     virtual ~UI();
//     std::vector<UIElement*> elements; //need pointer for identification (or do i??)
//     UIElement* addUIElement(double x0, double y0, double width, double height, glm::vec3 color);
//     void removeUIElement(UIElement* p_ui_window);
//     UIElement* p_clicked_ele; //smallest element, "inner most"

//     void mouse_click(double& x,double& y);
//     void mouse_release();
//     static UIElement* find_element_on_cursor(std::vector<UIElement*>& ui_windows, double& x, double& y);
// protected:
// private:
// };



// #endif // WORLDGEN_NIKITA_NEW_UI_H
