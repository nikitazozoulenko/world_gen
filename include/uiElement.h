#ifndef UIWINDOW_MOVABLE_H
#define UIWINDOW_MOVABLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>
#include <settings.h>



class UIElement
{
public:
    UIElement(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color,UIElement* p_parent=nullptr, std::string type="element");
    virtual ~UIElement();

    virtual void mouse_click(double& x, double& y);
    virtual void mouse_release();
    virtual void process_movement(double xoff, double yoff, double x, double y);
    double getGlobalX();
    double getGlobalY();

    Settings& settings;
    std::string type;
    UIElement* p_parent;
    std::vector<UIElement*> children;
    //(x0, y0, x0+w, y0+h) size in pixels
    double x0;
    double y0;
    double w;
    double h;
    //color in [0,1]^3
    glm::vec3 color;
    //other
    double time_last_press;
    double x_click;
    double y_click;
    double x0_at_click;
    double y0_at_click;
private:
};

class UIFrame : public UIElement
{
public:
    UIFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable, UIElement* p_parent=nullptr);
    ~UIFrame();
    void mouse_click(double& x, double& y);
    void mouse_release();
    void process_movement(double xoff, double yoff, double x, double y);

    bool moveable;
private:
};

class UISlider : public UIElement
{
public:
    UISlider(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, double tick_w, double line_h, 
        double min, double max, std::function<void(double, double)>& fun, UIElement* p_parent=nullptr);
    ~UISlider();
    void mouse_click(double& x, double& y);
    void mouse_release();
    void process_movement(double xoff, double yoff, double x, double y);

    void change_slider_val(double x, double y);

    // sizes in pixels
    double tick_w;
    double line_h;

    //other
    double min;
    double max;
    double value;
    std::function<void(double, double)> fun;
    bool tick_held_down;
    bool line_clicked;
    double tick_x0_at_click;

private:
};


    // void update_value(double xoff, double win_width);
    // bool x_on_tick(double x, double win_x0);
    // bool x_on_slider(double x, double win_x0);
    // bool find_if_on_slider(UIWindow* p_ui_window, double& x, double& y);

#endif // UIWINDOW_MOVABLE_H