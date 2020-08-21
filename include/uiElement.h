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
    UIFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable, 
        UIElement* p_parent=nullptr, std::string type="frame");
    virtual ~UIFrame();
    virtual void mouse_click(double& x, double& y);
    virtual void mouse_release();
    virtual void process_movement(double xoff, double yoff, double x, double y);

    bool moveable;
private:
};

class UIInvis : public UIFrame
{
public:
    UIInvis(Settings& settings, double x0, double y0, double w, double h, bool moveable, 
        UIElement* p_parent=nullptr, std::string type="invis");
private:
};

class UIScrollFrame : public UIFrame
{
public:
    UIScrollFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable,
        UIElement* p_parent=nullptr, std::string type="scrollframe");
    void scroll_move_win(double val, double change);
    double total_h;
};


class UISlider : public UIElement
{
public:
    UISlider(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, double tick_w, double line_h, 
        double min, double max, std::function<void(double, double)>& fun, UIElement* p_parent=nullptr, std::string type="slider");
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


class UIYSlider : public UIElement
{
public:
    UIYSlider(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, double tick_h, double line_w,
        double min, double max, std::function<void(double, double)>& fun, UIElement* p_parent=nullptr, std::string type="yslider");
    ~UIYSlider();
    void mouse_click(double& x, double& y);
    void mouse_release();
    void process_movement(double xoff, double yoff, double x, double y);

    void change_slider_val(double x, double y);

    // sizes in pixels
    double line_w;
    double tick_h;

    //other
    double min;
    double max;
    double value;
    std::function<void(double, double)> fun;
    bool tick_held_down;
    bool line_clicked;
    double tick_y0_at_click;

private:
};


class UIButton : public UIElement
{
public:
    UIButton(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, std::string button_text, 
            std::function<void()>& fun, UIElement* p_parent=nullptr, std::string type="button");
    ~UIButton();
    void mouse_click(double& x, double& y);
    void mouse_release();
    void process_movement(double xoff, double yoff, double x, double y);


    // sizes in pixels
    double tick_w;
    double line_h;

    //other
    std::string button_text;
    std::function<void()> fun;

private:
};




#endif // UIWINDOW_MOVABLE_H