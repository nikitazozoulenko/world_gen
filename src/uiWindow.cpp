#include <uiWindow.h>
#include <misc.h>

#include <algorithm>


UISlider::UISlider(float min, float max, glm::vec2 coords, float width, float height, float tick_width, float line_height):
    min(min),
    max(max),
    value((min+max)/2.0),
    held_down(false),
    coords(coords),
    width(width),
    height(height),
    tick_width(tick_width),
    line_height(line_height)
{

}



UIWindow::UIWindow(glm::vec2 coords, float width, float height, glm::vec3 color):
    coords(coords),
    width(width),
    height(height),
    color(color),
    time_last_press(glfwGetTime()),
    held_down(false)
{
    //how big a slider is
    float w = 0.44;
    float off_x = (1-2*w)/3.0;
    float h = 0.25;
    float off_y = (1-3*h)/4.0;

    float tick_w = 0.3;
    float line_h = 0.2;

    float min_val = 10;
    float max_val = 20;

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y), w, h, tick_w, line_h));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y), w, h, tick_w, line_h));

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y*2+h), w, h, tick_w, line_h));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y*2+h), w, h, tick_w, line_h));

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y*3 +h+h), w, h, tick_w, line_h));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y*3 +h+h), w, h, tick_w, line_h));
}



UIWindow* UIWindow::find_uiwindow_on_cursor(std::vector<UIWindow*>& ui_windows, float& x, float& y)
{   
    //find all ui_windows that are under the cursor to later pick the top most one.
    std::vector<int> window_indices;
    for(int i=0; i<ui_windows.size(); i++){   
        UIWindow* p_ui_window = ui_windows[i];
        float& x0 = p_ui_window->coords.x;
        float& y0 = p_ui_window->coords.y;
        float& width = p_ui_window->width;
        float& height = p_ui_window->height;

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



UISlider* UIWindow::find_if_on_slider(UIWindow* p_ui_window, UISlider& slider, float& x, float& y)
{
    float& w = p_ui_window->width;
    float& h = p_ui_window->height;
    float x0 = p_ui_window->coords.x + w*slider.coords.x;
    float y0 = p_ui_window->coords.y + h*slider.coords.y;
    float width = w*slider.width;
    float height = h*slider.height;

    //if (x,y) on slider
    if(x<(x0+width) && x0<x){
        if(y<(y0+height) && y0<y){
            return &slider;
        }
    }

    //if none found, return nullptr
    return nullptr;
}


UISlider* UIWindow::find_if_on_slider_tick(UIWindow* p_ui_window, UISlider& slider, float& x, float& y)
{
    if(find_if_on_slider(p_ui_window, slider, x, y)){
        //local (slider space is whole [0,1])
        float& min = slider.min;
        float& max = slider.max;
        float& value = slider.value;
        float tick_w = slider.tick_width;
        float c_val_norm = (value-min)/(max-min);
        float tick_x0 = c_val_norm * (1-tick_w);

        //local in window
        glm::vec2& coords = slider.coords;
        float& w = slider.width;
        tick_x0 = tick_x0*w + coords.x;
        tick_w = tick_w*w;

        //global
        glm::vec2& win_coords = p_ui_window->coords;
        float& win_width = p_ui_window->width;
        float line_w = w * win_width;
        tick_x0 = tick_x0*win_width + win_coords.x;
        tick_w = tick_w*win_width;

        if(tick_x0<x && x<(tick_x0+tick_w)){
            return &slider;
        }
    }

    //if none found, return nullptr
    return nullptr;
}



UISlider* UIWindow::find_slider_on_cursor(UIWindow* p_ui_window, float& x, float& y)
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



void UIWindow::uiwindow_mouse_move_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& xoffset, float& yoffset, float& x, float& y)
{
    //if mouse pressed (state is either PRESS or RELEASE)
    if (mouse_state == GLFW_PRESS)
    {
        UIWindow* p_pressed_uiwindow = find_uiwindow_on_cursor(ui_windows, x, y);
        if (p_pressed_uiwindow)
        {
            bool& held_down = p_pressed_uiwindow->held_down;
            float& x0 = p_pressed_uiwindow->coords.x;
            float& y0 = p_pressed_uiwindow->coords.y;
            float& window_w = p_pressed_uiwindow->width;
            float& window_h = p_pressed_uiwindow->height;

            //MOUSE HOLD LOGIC
            if(held_down)
            {
                UISlider* p_pressed_slider = p_pressed_uiwindow->p_pressed_slider;
                if(p_pressed_slider){
                    if(find_if_on_slider_tick(p_pressed_uiwindow, *p_pressed_slider, x,y)){
                    float& min = p_pressed_slider->min;
                    float& max = p_pressed_slider->max;
                    float& slider_w = p_pressed_slider->width;
                    float& tick_w = p_pressed_slider->tick_width;

                    float change = xoffset/slider_w/window_w/(1-tick_w)*(max-min);
                    float& val = p_pressed_slider->value;
                    val += change;
                    val = std::min(std::max(val, min), max);
                    }
                }
                else{
                    x0 += xoffset;
                    y0 += yoffset;
                }
            }
        }
    }
}



void UIWindow::uiwindow_click_callback(int& mouse_state, std::vector<UIWindow*>& ui_windows, float& x,float& y)
{
    if (mouse_state == GLFW_PRESS)
    {   
        //find window
        UIWindow* p_pressed_uiwindow = find_uiwindow_on_cursor(ui_windows, x, y);
        if (p_pressed_uiwindow)
        {   
            float t = glfwGetTime();
            p_pressed_uiwindow->held_down=true;
            p_pressed_uiwindow->time_last_press = t;

            //find slider
            UISlider* p_slider = find_slider_on_cursor(p_pressed_uiwindow, x, y);
            if(p_slider){
                p_slider->held_down = true;
                p_pressed_uiwindow->p_pressed_slider = p_slider;
            }
        }
    }
    else//if mouse_state == GLFW_RELEASE
    {
        for (auto& uiwindow : ui_windows)
        {//reset all was pressed last callback for next callback
            uiwindow->held_down = false;
            if(uiwindow->p_pressed_slider){
                uiwindow->p_pressed_slider->held_down = false;
                uiwindow->p_pressed_slider = nullptr;
            }
        }
    }
    
}