#include <uiElement.h>
#include <misc.h>
#include <scene.h>

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
void UIElement::mouse_release(double& x, double& y){print_float("base release",0);}
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

void UIElement::update_click_all_parents(double time, double& x, double& y)
{
    time_last_press=time;
    x_click =x;
    y_click =y;
    x0_at_click = getGlobalX();
    y0_at_click = getGlobalY();
    if(p_parent)
        p_parent->update_click_all_parents(time, x, y);
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
    else if(p_parent){
        p_parent->process_movement(xoff, yoff, x, y);
    }
}


////////////////////////////////////////////////


UIInvis::UIInvis(Settings& settings, double x0, double y0, double w, double h, bool moveable, UIElement* p_parent, std::string type):
    UIFrame(settings, x0, y0, w, h, {0,0,0}, moveable, p_parent, type)
{
}

////////////////////////////////////////////////

UIScrollFrame::UIScrollFrame(Settings& settings, double x0, double y0, double w, double h, glm::vec3 color, bool moveable, 
        UIElement* p_parent, std::string
         type):
    UIFrame(settings, x0, y0, w, h, color, moveable, p_parent, type),
    total_h(0)
{
}

void UIScrollFrame::scroll_move_win(double val, double change)
{
    for(auto p_child : children){
        p_child->y0 += change;
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
void UISlider::mouse_release(double& x, double& y)
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
void UIYSlider::mouse_release(double& x, double& y)
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
void UIButton::mouse_release(double& x, double& y)
{
}
void UIButton::process_movement(double xoff, double yoff, double x, double y)
{
}



////////////////////////////////////////////////



UIBlockItem::UIBlockItem(Settings& settings, double x0, double y0, double w, double h, double border_size, glm::vec3 border_col, glm::vec3 item_col, 
        unsigned int& block, bool changeable, UI_FreeCamWorld* p_ui, UIElement* p_parent, std::string type) :
    UIElement(settings, x0, y0, w, h, border_col, p_parent, type),
    p_ui(p_ui),
    border_col(border_col),
    item_col(item_col),
    border_size(border_size),
    block(block),
    changeable(changeable)
{
}
void UIBlockItem::mouse_click(double& x, double& y)
{
    double size = w-2*border_size;
    UIElement* p_block = new UIPickedBlock(settings, x-size/2.0, y-size/2.0, size, size,
                                    item_col, block, p_ui);
    p_ui->elements.push_back(p_block);
    p_ui->p_clicked_ele = p_block;
    p_block->update_click_all_parents(glfwGetTime(), x, y);
    p_block->x_click = p_block->x0_at_click+p_block->w/2.0;
    p_block->y_click = p_block->y0_at_click+p_block->h/2.0;

}


UIPickedBlock::UIPickedBlock(Settings& settings, double x0, double y0, double w, double h, glm::vec3 item_col, unsigned int block, 
            UI_FreeCamWorld* p_ui, std::string type) :
    UIFrame(settings, x0, y0, w, h, item_col, true, nullptr, type),
    item_col(item_col),
    block(block),
    p_ui(p_ui)
{

}
void UIPickedBlock::mouse_release(double& x, double& y)
{
    //change block if its okay
    if(p_ui->elements_on_cursor.size()>1){
        UIBlockItem* p_under = static_cast<UIBlockItem*>(p_ui->elements_on_cursor.at(1));
        if(p_under->type=="blockitem"){
            if(p_under->changeable){
                p_under->block=block;
            }
        }
    }

    //now delete this hovering uielement
    auto f = std::find(p_ui->elements.begin(), p_ui->elements.end(), this);
    if(f!=p_ui->elements.end()){
        p_ui->elements.erase(f);
    }
    delete this;
}

