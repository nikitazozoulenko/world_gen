#include <algorithm>

#include <ui.h>
#include <scene.h>
#include <misc.h>


UI::UI(Settings& settings, Scene* p_scene) : settings(settings), p_scene(p_scene), p_clicked_ele(nullptr)
{
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

UI_FreeCamWorld::UI_FreeCamWorld(Settings& settings, FreeCamWorld* p_scene) : UI(settings, p_scene)
{
}
UI_FreeCamWorld::~UI_FreeCamWorld()
{
}

void UI_FreeCamWorld::create_ui() //WARNING: need to create p_scene->slider_functions first
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

    UISlider* p_child1 = new UISlider(settings, off_x, off_y, w, h, glm::vec3(54/255.0, 54/255.0, 99/255.0), 
                                        tick_w, line_h, m, M, p_scene->slider_functions.at("test"), p_frame);
    UIButton* p_child2 = new UIButton(settings, off_x, off_y*2+h, w, h, glm::vec3(66/255.0, 66/255.0, 66/255.0),
                                        "Menu", p_scene->button_functions.at("mainmenu"), p_frame);

    // UIFrame* p_child1 = new UIFrame(settings, 50, 50, 200, 200, 0.75f*glm::vec3(54/255.0, 54/255.0, 99/255.0), true, p_frame);
    // UIFrame* p_child2 = new UIFrame(settings, 25, 25, 150, 150, 0.5f*glm::vec3(54/255.0, 54/255.0, 99/255.0), true, p_child1);

    //cursor fake
    UIFrame* p_cursor = new UIFrame(settings, 1280/2-1, 720/2-1, 2, 2, glm::vec3(1,1,1), true);
    elements.push_back(p_cursor);
}


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////// MainMenu //////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UI_MainMenu::UI_MainMenu(Settings& settings, MainMenu* p_scene) : UI(settings, p_scene)
{
}
UI_MainMenu::~UI_MainMenu()
{
}

void UI_MainMenu::create_ui() //WARNING: need to create p_scene->slider_functions first
{
    int n_buttons=3;
    double w =400;
    double h =100;
    double win_w = settings.getWindowWidth();
    double win_h = settings.getWindowHeight();
    glm::vec3 button_color(0.4, 0.8, 0.2);

    double off_x = (win_w-w)/2.0;
    double off_y = (win_h-n_buttons*h)/(n_buttons+1);

    UIButton* p_button_world = new UIButton(settings, off_x, off_y*1+h*0, w, h, button_color, 
                                "Exit", p_scene->button_functions.at("exit"));
    elements.push_back(p_button_world);
    UIButton* p_button_editor = new UIButton(settings, off_x, off_y*2+h*1, w, h, button_color, 
                                "Editor", p_scene->button_functions.at("editor"));
    elements.push_back(p_button_editor);
    UIButton* p_button_exit = new UIButton(settings, off_x, off_y*3+h*2, w, h, button_color, 
                                "Create World", p_scene->button_functions.at("freecamworld"));
    elements.push_back(p_button_exit);
}



// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////// Editor //////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UI_Editor::UI_Editor(Settings& settings, Editor* p_scene) : 
    UI(settings, p_scene), 
    win_per_row(3)
{
}
UI_Editor::~UI_Editor()
{
}

void UI_Editor::create_ui() //WARNING: need to create p_scene->slider_functions first
{
    int n_buttons=1;
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();
    double frame_w =screen_w-150; //1130
    double frame_h =screen_h-150; //570
    double frame_x = (screen_w-frame_w)/2.0;
    double frame_y = (screen_h-n_buttons*frame_h)/(n_buttons+1);

    UIEditorFrame* p_frame = new UIEditorFrame(settings, frame_x, frame_y, frame_w, frame_h, glm::vec3(2/255.0, 33/255.0, 44/255.0), 
                                    false, nullptr, "editorframe");
    elements.push_back(p_frame);

    //slider
    double slider_space =20;
    double tick_w=70;
    double tick_h=40; // this depends on how many windows
    double line_w=20;
    double line_h=frame_h-2*slider_space;

    //windows
    double window_space = 45;
    double win_ratio = 3.0/4.0;
    double win_w = (frame_w - (slider_space + tick_w + slider_space + (win_per_row-1)*window_space + slider_space))/win_per_row;
    double win_h = win_w*win_ratio;



    double win_start_x = slider_space + tick_w + slider_space;
    double win_start_y = frame_h - win_h - slider_space;
    double win_off_x = win_w+window_space;
    double win_off_y = -win_h-window_space;
    glm::vec3 win_color(200/255.0, 200/255.0, 2/255.0);

    int n_windows = 8;
    for(int i=0; i<n_windows; i++)
    {
        int row = i/win_per_row;
        int col = i%win_per_row;
        new UIFrame(settings, win_start_x+col*win_off_x, win_start_y+row*win_off_y, win_w, win_h, win_color, true, p_frame);
    }

    
    total_h = 2*slider_space-window_space + -(std::ceil(n_windows/(double)win_per_row)*win_off_y);
    total_h = std::max(frame_h, total_h);
    std::function<void(double, double)> slider_fun= std::bind(&UIEditorFrame::scroll_move_win, p_frame,std::placeholders::_1, std::placeholders::_2);
    UIYSlider* p_slider = new UIYSlider(settings, slider_space, slider_space, tick_w, line_h, glm::vec3(54/255.0, 222/255.0, 99/255.0), 
                                       tick_h, line_w, 0, total_h-frame_h, 
                                       slider_fun, 
                                       p_frame);
    p_slider->value=p_slider->max;



}