#include <uiElement.h>
#include <misc.h>

#include <algorithm>


UIElement::UIElement(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, UIElement* p_parent, std::string type):
    x0(x0),y0(y0),w(w),h(h),color(color),type(type),settings(settings),
    time_last_press(glfwGetTime()),
    p_parent(p_parent),
    children()
{
    if(p_parent){
        p_parent->children.push_back(this);
    }
}

UIElement::~UIElement()
{
    for(auto& ele : children){
        delete ele;
    }
}

void UIElement::mouse_click(double& x, double& y){print_float("base click",0);}
void UIElement::mouse_release(){print_float("base release",0);}
void UIElement::process_movement(double xoff, double yoff, double x, double y){print_float("base process",0);}

double UIElement::getGlobalX()
{
    if(p_parent){
        return x0+p_parent->getGlobalX();
    }else{
        return x0;
    }
}
double UIElement::getGlobalY()
{
    if(p_parent){
        return y0+p_parent->getGlobalY();
    }else{
        return y0;
    }
}

////////////////////////////////////////////////

UIFrame::UIFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable, 
        UIElement* p_parent, std::string type):
    UIElement(settings, x0, y0, w, h, color, p_parent, type),
    moveable(moveable)
{

}

UIFrame::~UIFrame()
{
}

void UIFrame::mouse_click(double& x, double& y)
{
}
void UIFrame::mouse_release()
{
}
void UIFrame::process_movement(double xoff, double yoff, double x, double y)
{
    double W = settings.getWindowWidth();
    double H = settings.getWindowHeight();
    double OFFX=0.0;
    double OFFY=0.0;
    if(p_parent){
        W = p_parent->w;
        H = p_parent->h;
        OFFX=p_parent->x0_at_click;
        OFFY=p_parent->y0_at_click;
    }
    if(moveable){
        //clamp = std::min( maxval, std::max(minval, val) );
        x0=std::min(W-(x0_at_click+w-x_click), std::max(x_click-x0_at_click, x-OFFX)) - x_click+x0_at_click;
        y0=std::min(H-(y0_at_click+h-y_click), std::max(y_click-y0_at_click, y-OFFY)) - y_click+y0_at_click;
    }
}

////////////////////////////////////////////////

UIEditorFrame::UIEditorFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable, 
        UIElement* p_parent, std::string type):
    UIFrame(settings, x0, y0, w, h, color, moveable, p_parent, type)
{
}

void UIEditorFrame::scroll_move_win(double val, double change)
{
    for(auto p_child : children){
        if(p_child->type!="yslider"){
            p_child->y0 += change;
        }
    }
}


////////////////////////////////////////////////


UISlider::UISlider(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, double tick_w, double line_h, 
        double min, double max, std::function<void(double, double)>& fun, UIElement* p_parent, std::string type):
    UIElement(settings, x0, y0, w, h, color, p_parent, type), 
    tick_w(tick_w), line_h(line_h), min(min), max(max), fun(fun),
    value((max+min)/2.0),
    tick_held_down(false),
    line_clicked(false)
{
}

UISlider::~UISlider()
{
}

void UISlider::change_slider_val(double x, double y)
{
    double temp_val = value;

    //clamp = std::min( maxval, std::max(minval, val) );
    double minval = x_click-tick_x0_at_click;
    double maxval = w-(tick_x0_at_click+tick_w-x_click);
    double X0=std::min(maxval - minval, std::max(0.0, x - x0_at_click - minval));
    value = X0*(max-min)/(maxval-minval) + min;

    if( value!=temp_val ){
        fun(value, temp_val-value);
    }
}

void UISlider::mouse_click(double& x, double& y)
{
    double c_val_norm = (value-min)/(max-min);
    double tick_x0 = x0_at_click + c_val_norm*(w-tick_w);
    double& tick_y0 = y0_at_click;
    double& tick_h = h;

    double& line_x0 = x0_at_click;
    double line_y0 = y0_at_click + 0.5*(h - line_h);
    double& line_w = w;

    //if on slider tick
    if(tick_x0 < x && x < tick_x0+tick_w && tick_y0 < y && y < tick_y0+tick_h)
    {
        tick_held_down=true;
        tick_x0_at_click = x0_at_click + c_val_norm*(w-tick_w);
    }//if on line
    else if(line_x0 < x && x < line_x0+line_w && line_y0 < y && y < line_y0+line_h)
    {
        line_clicked=true;
        x_click = tick_x0_at_click + tick_w/2;
        change_slider_val(x, y);
        tick_held_down=true;
    }
}
void UISlider::mouse_release()
{
    tick_held_down=false;
    line_clicked=false;
}
void UISlider::process_movement(double xoff, double yoff, double x, double y)
{
    if(tick_held_down){
        change_slider_val(x,y);
    }
    else if(!line_clicked){
        p_parent->process_movement(xoff, yoff, x, y);
    }
}




////////////////////////////////////////////////


UIYSlider::UIYSlider(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, double tick_h, double line_w,
        double min, double max, std::function<void(double, double)>& fun, UIElement* p_parent, std::string type):
    UIElement(settings, x0, y0, w, h, color, p_parent, type), 
    tick_h(tick_h), line_w(line_w), min(min), max(max), fun(fun),
    value((max+min)/2.0),
    tick_held_down(false),
    line_clicked(false)
{
}

UIYSlider::~UIYSlider()
{
}

void UIYSlider::change_slider_val(double x, double y)
{
    double temp_val = value;

    //clamp = std::min( maxval, std::max(minval, val) );
    double minval = y_click-tick_y0_at_click;
    double maxval = h-(tick_y0_at_click+tick_h-y_click);
    double Y0=std::min(maxval - minval, std::max(0.0, y - y0_at_click - minval));
    value = Y0*(max-min)/(maxval-minval) + min;

    if( value!=temp_val ){
        fun(value, temp_val-value);
    }
}

void UIYSlider::mouse_click(double& x, double& y)
{
    double c_val_norm = (value-min)/(max-min);
    double tick_y0 = y0_at_click + c_val_norm*(h-tick_h);
    double& tick_x0 = x0_at_click;
    double& tick_w = w;

    double& line_y0 = y0_at_click;
    double line_x0 = x0_at_click + 0.5*(w - line_w);
    double& line_h = h;

    //if on slider tick
    if(tick_x0 < x && x < tick_x0+tick_w && tick_y0 < y && y < tick_y0+tick_h)
    {
        tick_held_down=true;
        tick_y0_at_click = y0_at_click + c_val_norm*(h-tick_h);
    }//if on line
    else if(line_x0 < x && x < line_x0+line_w && line_y0 < y && y < line_y0+line_h)
    {
        line_clicked=true;
        y_click = tick_y0_at_click + tick_h/2;
        change_slider_val(x, y);
        tick_held_down=true;
    }
}
void UIYSlider::mouse_release()
{
    tick_held_down=false;
    line_clicked=false;
}
void UIYSlider::process_movement(double xoff, double yoff, double x, double y)
{
    if(tick_held_down){
        change_slider_val(x,y);
    }
    else if(!line_clicked){
        p_parent->process_movement(xoff, yoff, x, y);
    }
}




////////////////////////////////////////////////


UIButton::UIButton(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, std::string button_text, 
            std::function<void()>& fun,  UIElement* p_parent, std::string type):
    UIElement(settings, x0, y0, w, h, color, p_parent, type), 
    button_text(button_text), fun(fun)
{
}

UIButton::~UIButton()
{
}


void UIButton::mouse_click(double& x, double& y)
{
    fun();
}
void UIButton::mouse_release()
{
}
void UIButton::process_movement(double xoff, double yoff, double x, double y)
{
}
