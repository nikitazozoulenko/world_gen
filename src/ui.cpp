#include <algorithm>

#include <ui.h>
#include <scene.h>


UI::UI(Settings& settings, Scene* p_scene) : settings(settings), p_scene(p_scene), p_clicked_ele(nullptr)
{
}

void UI::create_ui() //WARNING: need to create p_scene->slider_functions first
{
    double win_w = 300;
    double win_h = 300;
    UIElement* p_frame = new UIFrame(settings, 100, 100, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0), true);
    elements.push_back(p_frame);


    //how big a slider is
    double w = 200;
    double off_x = (win_w-w)/2.0;
    double h = 100;
    double off_y = (win_h-2*h)/3.0;

    double tick_w = 50;
    double line_h = 40;

    double m = 30;
    double M = 200;

    UIElement* p_child1 = new UISlider(settings, off_x, off_y, w, h, glm::vec3(54/255.0, 54/255.0, 99/255.0), 
                                        tick_w, line_h, m, M, p_scene->slider_functions.at("test"), p_frame);
    UIElement* p_child2 = new UISlider(settings, off_x, off_y*2+h, w, h, glm::vec3(54/255.0, 54/255.0, 99/255.0),
                                        tick_w, line_h, m, M, p_scene->slider_functions.at("test"), p_frame);

    // UIFrame* p_child1 = new UIFrame(settings, 50, 50, 200, 200, 0.75f*glm::vec3(54/255.0, 54/255.0, 99/255.0), true, p_frame);
    // UIFrame* p_child2 = new UIFrame(settings, 25, 25, 150, 150, 0.5f*glm::vec3(54/255.0, 54/255.0, 99/255.0), true, p_child1);

    //cursor fake
    UIFrame* p_cursor = new UIFrame(settings, 1280/2-1, 720/2-1, 2, 2, glm::vec3(1,1,1), true);
    elements.push_back(p_cursor);
}


UI::~UI()
{
    for(auto& ele : elements){
        delete ele;
    }
}

UIElement* UI::find_element_on_cursor(std::vector<UIElement*>& elements, double x, double y)
{   
    //find all UIElements that are under the cursor to later pick the top most one.
    std::vector<int> ele_indices;
    for(int i=0; i<elements.size(); i++){   
        UIElement* p_element = elements[i];
        double& x0 = p_element->x0;
        double& y0 = p_element->y0;
        double& w = p_element->w;
        double& h = p_element->h;

        //if (x,y) in ui window
        if(x<(x0+w) && x0<x){
            if(y<(y0+h) && y0<y){
                ele_indices.push_back(i);
            }
        }
    }

    //now pick the top most window
    if(ele_indices.size()>0){
        std::sort(ele_indices.begin(), ele_indices.end(), [&elements](const auto& lhs, const auto& rhs){return elements[lhs]->time_last_press > elements[rhs]->time_last_press;});
        return elements[ele_indices[0]];
    }
    //if none found, return nullptr
    return nullptr;
}

void UI::mouse_click(double x, double y)
{
    //find window
    p_clicked_ele = nullptr;
    UIElement* p_ele=nullptr;
    std::vector<UIElement*> iter_elements = elements;
    double off_x =0;
    double off_y =0;
    double time = glfwGetTime();
    do
    {
        p_ele = find_element_on_cursor(iter_elements, x-off_x, y-off_y);
        if(p_ele){
            iter_elements = p_ele->children;
            p_clicked_ele = p_ele;
            off_x += p_ele->x0;
            off_y += p_ele->y0;
            p_ele->time_last_press=time;
            p_ele->x_click=x;
            p_ele->y_click=y;
            p_ele->x0_at_click=off_x;
            p_ele->y0_at_click=off_y;
            
        }
    } while (p_ele!=nullptr && !iter_elements.empty());
    
    if(p_clicked_ele){
        p_clicked_ele->mouse_click(x, y);
    }
}


void UI::mouse_release()
{
    if(p_clicked_ele){
        p_clicked_ele->mouse_release();
        p_clicked_ele = nullptr;
    }
}


void UI::process_movement(double xoff, double yoff, double x, double y)
{
    if(p_clicked_ele){
        p_clicked_ele->process_movement(xoff, yoff, x, y);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// FreeCamWorld //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UI_FreeCamWorld::UI_FreeCamWorld(FreeCamWorld* p_scene) :
//     p_scene(p_scene)
// {
// }



// void UI_FreeCamWorld::createUI()
// {
//     double win_w = 300;
//     double win_h = 300;
//     UIWindow* p_window = createUIWindow(100, 100, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0));
//     std::vector<UISlider>& sliders = p_window->sliders;

//     //how big a slider is
//     double w = 200;
//     double off_x = (win_w-w)/2.0;
//     double h = 100;
//     double off_y = (win_h-2*h)/3.0;

//     double tick_w = 50;
//     double line_h = 40;

//     double m_size = 2;
//     double M_size = 100;
//     double m_amp = 0;
//     double M_amp = 2;
    
//     std::function<void(double, double)> fun_amp = std::bind(&FreeCamWorld::slider_amp_fun, p_scene, std::placeholders::_1, std::placeholders::_2);
//     std::function<void(double, double)> fun_size = std::bind(&FreeCamWorld::slider_size_fun, p_scene, std::placeholders::_1, std::placeholders::_2);

//     sliders.push_back(UISlider(m_size, M_size, off_x, off_y, w, h, tick_w, line_h, fun_size));
//     sliders.push_back(UISlider(m_amp, M_amp, off_x, off_y*2+h, w, h, tick_w, line_h, fun_amp));



//     //cursor fake
//     createUIWindow(1280/2-1, 720/2-1, 2, 2, glm::vec3(1,1,1));
// }

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////// MainMenu //////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UI_MainMenu::UI_MainMenu(MainMenu* p_scene) :
//     p_scene(p_scene)
// {

// }

// void UI_MainMenu::createUI()
// {
//     double win_w = 700;
//     double win_h = 400;
//     UIWindow* p_window = createUIWindow(200, 100, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0));
//     std::vector<UISlider>& sliders = p_window->sliders;

//     //how big a slider is
//     double w = 300;
//     double off_x = (win_w-2*w)/3.0;
//     double h = 100;
//     double off_y = (win_h-3*h)/4.0;

//     double tick_w = 50;
//     double line_h = 20;

//     double min_val = 10;
//     double max_val = 20;
    
//     std::function<void(double, double)> fun = std::bind(&MainMenu::test, p_scene, std::placeholders::_1, std::placeholders::_2);

//     sliders.push_back(UISlider(min_val, max_val, off_x, off_y, w, h, tick_w, line_h, fun));
//     sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y, w, h, tick_w, line_h, fun));

//     sliders.push_back(UISlider(min_val, max_val, off_x, off_y*2+h, w, h, tick_w, line_h, fun));
//     sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y*2+h, w, h, tick_w, line_h, fun));

//     sliders.push_back(UISlider(min_val, max_val, off_x, off_y*3 +h+h, w, h, tick_w, line_h, fun));
//     sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y*3 +h+h, w, h, tick_w, line_h, fun));
// }
