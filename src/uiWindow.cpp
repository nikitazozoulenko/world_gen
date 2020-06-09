#include <uiWindow.h>
#include <misc.h>

#include <algorithm>


UISlider::UISlider(double min, double max, double x0, double y0, double width, double height, double tick_width, double line_height, 
                    std::function<void(double, double)>& fun):
    min(min),
    max(max),
    value((min+max)/2.0),
    x0(x0),
    y0(y0),
    width(width),
    height(height),
    tick_width(tick_width),
    line_height(line_height),
    fun(fun),
    tick_held_down(false)
{
    fun(value, 0);
}


void UISlider::update_value(double xoff, double win_width) //width is window width where slider is in
{
    double change = xoff/width/win_width/(1-tick_width)*(max-min);
    double new_value = value + change;
    new_value = std::min(std::max(new_value, min), max);
    value=new_value;
    if(new_value!=value){
        fun(new_value, new_value-value);
    }
}


bool UISlider::x_on_tick(double x, double win_x0, double win_width)
{
    //local (slider space is whole [0,1])
    double tick_w = tick_width;
    double c_val_norm = (value-min)/(max-min);
    double tick_x0 = c_val_norm * (1-tick_w);

    //local in window
    double& w = width;
    tick_x0 = tick_x0*w + x0;
    tick_w = tick_w*w;

    //global
    double line_w = w * win_width;
    tick_x0 = tick_x0*win_width + win_x0;
    tick_w = tick_w*win_width;

    if(tick_x0<x && x<(tick_x0+tick_w)){
        return true;
    }
    return false;
}
bool UISlider::x_on_slider(double x, double win_x0, double win_width)
{
    double line_x0 = x0*win_width + win_x0;

    if(line_x0<x && x<(line_x0+width*win_width)){
        return true;
    }
    return false;
}



//////////////////////////////////////////////////////////////////////////////////////////////////


UIWindow::UIWindow(double x0, double y0, double width, double height, glm::vec3 color):
    x0(x0),
    y0(y0),
    width(width),
    height(height),
    color(color),
    time_last_press(glfwGetTime()),
    p_pressed_slider(nullptr)
{

}



UIWindow* UIWindow::find_uiwindow_on_cursor(std::vector<UIWindow*>& ui_windows, double& x, double& y)
{   
    //find all ui_windows that are under the cursor to later pick the top most one.
    std::vector<int> window_indices;
    for(int i=0; i<ui_windows.size(); i++){   
        UIWindow* p_ui_window = ui_windows[i];
        double& x0 = p_ui_window->x0;
        double& y0 = p_ui_window->y0;
        double& width = p_ui_window->width;
        double& height = p_ui_window->height;

        //if (x,y) in window
        if(x<(x0+width) && x0<x){
            if(y<(y0+height) && y0<y){
                window_indices.push_back(i);
            }
        }
    }

    //now pick the top most window
    if(window_indices.size()>0){
        std::sort(window_indices.begin(), window_indices.end(), [&ui_windows](const auto& lhs, const auto& rhs){return ui_windows[lhs]->time_last_press > ui_windows[rhs]->time_last_press;});
        return ui_windows[window_indices[0]];
    }
    //if none found, return nullptr
    return nullptr;
}



UISlider* UIWindow::find_if_on_slider(UIWindow* p_ui_window, UISlider& slider, double& x, double& y)
{
    double& w = p_ui_window->width;
    double& h = p_ui_window->height;
    double x0 = p_ui_window->x0 + w*slider.x0;
    double y0 = p_ui_window->y0 + h*slider.y0;
    double width = w*slider.width;
    double height = h*slider.height;

    //if (x,y) on slider
    if(x<(x0+width) && x0<x){
        if(y<(y0+height) && y0<y){
            return &slider;
        }
    }

    //if none found, return nullptr
    return nullptr;
}


UISlider* UIWindow::find_if_on_slider_tick(UIWindow* p_ui_window, UISlider& slider, double& x, double& y)
{
    if(find_if_on_slider(p_ui_window, slider, x, y)){
        //local (slider space is whole [0,1])
        double& min = slider.min;
        double& max = slider.max;
        double& value = slider.value;
        double tick_w = slider.tick_width;
        double c_val_norm = (value-min)/(max-min);
        double tick_x0 = c_val_norm * (1-tick_w);

        //local in window
        double& w = slider.width;
        tick_x0 = tick_x0*w + slider.x0;
        tick_w = tick_w*w;

        //global
        double& x0 = p_ui_window->x0;
        double& win_width = p_ui_window->width;
        double line_w = w * win_width;
        tick_x0 = tick_x0*win_width + x0;
        tick_w = tick_w*win_width;

        if(tick_x0<x && x<(tick_x0+tick_w)){
            return &slider;
        }
    }

    //if none found, return nullptr
    return nullptr;
}



UISlider* UIWindow::find_slider_on_cursor(UIWindow* p_ui_window, double& x, double& y)
{   
    //loop through and return pointer to slider
    for(auto& slider : p_ui_window->sliders){
        UISlider* p_slider = find_if_on_slider(p_ui_window, slider, x, y);
        if(p_slider){
            return p_slider;
        }
    }

    //if none found, return nullptr
    return nullptr;
}


void UIWindow::uiwindow_click(std::vector<UIWindow*>& ui_windows, double& x,double& y,UIWindow** p_inputscheme_pressed_window)
{
    //find window
    UIWindow* p_pressed_uiwindow = find_uiwindow_on_cursor(ui_windows, x, y);
    if (p_pressed_uiwindow)
    {   
        *p_inputscheme_pressed_window = p_pressed_uiwindow;
        p_pressed_uiwindow->time_last_press = glfwGetTime();

        //find slider
        UISlider* p_slider = find_slider_on_cursor(p_pressed_uiwindow, x, y);
        if(p_slider){
            p_pressed_uiwindow->p_pressed_slider = p_slider;
            if(p_slider->x_on_tick(x, p_pressed_uiwindow->x0, p_pressed_uiwindow->width)){
                p_slider->tick_held_down = true;
            }
        }
    }

}


void UIWindow::uiwindow_release(UIWindow** p_inputscheme_pressed_window)
{
    UIWindow* p_pressed_window = *p_inputscheme_pressed_window;
    if(p_pressed_window){
        if(p_pressed_window->p_pressed_slider){
            p_pressed_window->p_pressed_slider->tick_held_down=false;
        }
        p_pressed_window->p_pressed_slider = nullptr;
    }
    *p_inputscheme_pressed_window = nullptr;
}


void UIWindow::process_movement(double xoff, double yoff, double x, double y)
{
    //if on slider tick
    if(p_pressed_slider){
        if(p_pressed_slider->tick_held_down && p_pressed_slider->x_on_slider(x, x0, width)){
            p_pressed_slider->update_value(xoff, width);
        }
    }
    else{
        x0 += xoff;
        y0 += yoff;
    }
}