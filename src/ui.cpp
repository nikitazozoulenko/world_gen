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

UI_FreeCamWorld::UI_FreeCamWorld(Settings& settings, FreeCamWorld* p_scene) : UI(settings, p_scene), p_itempick_base(nullptr), p_escapemenu(nullptr)
{
}
UI_FreeCamWorld::~UI_FreeCamWorld()
{
}

void UI_FreeCamWorld::create_ui() //WARNING: need to create p_scene->slider_functions first
{
    create_itempick();
    create_escapemenu();

    elements.push_back(p_cursor);
}


void UI_FreeCamWorld::create_escapemenu()
{
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();

    double win_w = 300;
    double win_h = 300;
    double start_x = (screen_w-win_w)/2.0;
    double start_y = (screen_h-win_h)/2.0;
    p_escapemenu = new UIFrame(settings, start_x, start_y, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0), true);

    //how big a slider is
    double w = 200;
    double off_x = (win_w-w)/2.0;
    double h = 75;
    double off_y = (win_h-3*h)/4.0;
    glm::vec3 col(66/255.0, 66/255.0, 66/255.0);

    UIButton* p_child1 = new UIButton(settings, off_x, off_y, w, h, col,
                                        "Main Menu", p_scene->button_functions.at("mainmenu"), p_escapemenu);
    UIButton* p_child2 = new UIButton(settings, off_x, off_y*2+h, w, h, col,
                                        "TODO", p_scene->button_functions.at("nothing"), p_escapemenu);
    UIButton* p_child3 = new UIButton(settings, off_x, off_y*3+h+h, w, h, col,
                                        "Exit", p_scene->button_functions.at("exit"), p_escapemenu);


    //cursor fake
    p_cursor = new UIFrame(settings, 1280/2-1, 720/2-1, 2, 2, glm::vec3(1,1,1), true);
    elements.push_back(p_cursor);
}


void UI_FreeCamWorld::create_itempick()
{
    //start options
    double hotbar_item_size = 50;
    double hotbar_space_to_edges = 10;
    double hotbar_item_space = 30;
    double space_between_frame_hotbar = 10; //between hotbar and select
    double frame_space = 20;
    double slider_w = 30;
    double search_w = 160;
    double search_h = 40;
    glm::vec3 search_col(0.3, 0.3, 0.3);
    glm::vec3 frame_col(0.4, 0.4, 0.4);
    glm::vec3 select_col(0.5, 0.5, 0.5);
    glm::vec3 item_color(200/255.0, 200/255.0, 2/255.0);
    glm::vec3 slider_col(0.6, 0.6, 0.6);
    int n_hotbar_items = settings.hotbar_size;
    int n_select_row_size = settings.inv_row_size;
    int n_select_col_size = settings.inv_col_size;
    double screen_w = settings.getWindowWidth();
    double screen_h = settings.getWindowHeight();

    //base=frame+hotbar


    double base_w = (n_hotbar_items-1)*hotbar_item_space + n_hotbar_items*hotbar_item_size + 2*hotbar_space_to_edges;
    double hotbar_h = hotbar_item_size + 2*hotbar_space_to_edges;
    double select_item_size = (base_w-((4+n_select_row_size)*frame_space + slider_w))/(double)n_select_row_size;
    double slider_h = n_select_col_size*(frame_space + select_item_size) + frame_space;
    double frame_h = 3*frame_space + slider_h + search_h;
    double start_x = (screen_w-base_w)/2.0;
    double start_y = (screen_h-frame_h)/2.0;
    p_itempick_base = new UIInvis(settings, start_x, start_y, base_w, hotbar_h+space_between_frame_hotbar+frame_h, true);
    UIFrame* p_hotbar = new UIFrame(settings, 0, 0, base_w, hotbar_h, frame_col, false, p_itempick_base);
    UIFrame* p_frame = new UIFrame(settings, 0, hotbar_h+space_between_frame_hotbar, base_w, frame_h, frame_col, false, p_itempick_base);

    //select
    UIScrollFrame* p_select = new UIScrollFrame(settings, 2*frame_space+slider_w, frame_space, base_w-(3*frame_space+slider_w), slider_h, select_col, false, p_frame);
    double item_start_x = frame_space;
    double item_start_y = slider_h - select_item_size - frame_space;
    double item_off_x = select_item_size+frame_space;
    double item_off_y = -select_item_size-frame_space;
    int n_windows = n_select_row_size * n_select_col_size*3;
    for(int i=0; i<n_windows; i++)
    {
        int row = i/n_select_row_size;
        int col = i%n_select_row_size;
        new UIFrame(settings, item_start_x+col*item_off_x, item_start_y+row*item_off_y, select_item_size, select_item_size, item_color, true, p_select);
    }
    p_select->total_h = frame_space + std::ceil(n_windows/(double)-n_select_row_size)*item_off_y;
    p_select->total_h = std::max(slider_h, p_select->total_h);

    //slider
    std::function<void(double, double)> slider_fun= std::bind(&UIScrollFrame::scroll_move_win, p_select, std::placeholders::_1, std::placeholders::_2);
    UIYSlider* p_slider = new UIYSlider(settings, frame_space, frame_space, slider_w, slider_h, slider_col, 
                                       slider_w, slider_w, 0, p_select->total_h-slider_h, slider_fun, p_frame);
    p_slider->value=p_slider->max;

    //search
    UIButton* p_search = new UIButton(settings, 2*frame_space+slider_w, 2*frame_space+slider_h, search_w, search_h, search_col, "Search...", p_scene->button_functions.at("nothing"), p_frame);

    //hotbar items
    double hot_off = hotbar_item_space+hotbar_item_size;
    for(int i=0; i<n_hotbar_items; i++){
        new UIFrame(settings, hotbar_space_to_edges+i*hot_off, hotbar_space_to_edges, hotbar_item_size, hotbar_item_size, item_color, true, p_itempick_base);
    }
}

void UI_FreeCamWorld::toggle_menu()
{
    toggle_element(p_escapemenu);
}
void UI_FreeCamWorld::toggle_inventory()
{
    toggle_element(p_itempick_base);
}
void UI_FreeCamWorld::toggle_element(UIElement* p_ele)
{
    auto f = std::find(elements.begin(), elements.end(), p_ele);
    if(f!=elements.end()){
        elements.erase(f);
    }else{
        p_ele->time_last_press=glfwGetTime();
        elements.push_back(p_ele);
    }

}

bool UI_FreeCamWorld::is_cursor_mode()
{
    std::vector<UIElement*> eles_with_cursor{p_itempick_base, p_escapemenu};
    bool needs_cursor_mode=false;
    for(UIElement* p_ele : eles_with_cursor){
        if(std::find(elements.begin(), elements.end(), p_ele) != elements.end()){
            needs_cursor_mode=true;
            break;
        }
    }
    return needs_cursor_mode;
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
    glm::vec3 frame_col(2/255.0, 33/255.0, 44/255.0);

    UIFrame* p_frame = new UIFrame(settings, frame_x, frame_y, frame_w, frame_h, frame_col,false);
    elements.push_back(p_frame);

    //slider
    double space =20;
    double tick_w=70;
    double tick_h=40; // this depends on how many windows
    double line_w=20;
    double line_h=frame_h-2*space;

    //windows
    double window_space = 45;
    double win_ratio = 3.0/4.0;
    double win_w = (frame_w - (space + tick_w + space + (win_per_row-1)*window_space + space))/win_per_row;
    double win_h = win_w*win_ratio;



    double win_start_x = space;
    double win_start_y = frame_h - win_h - space;
    double win_off_x = win_w+window_space;
    double win_off_y = -win_h-window_space;
    glm::vec3 win_color(200/255.0, 200/255.0, 2/255.0);

    UIScrollFrame* p_scrollframe = new UIScrollFrame(settings, space+tick_w, 0, frame_w-(space+tick_w), frame_h, frame_col,
                                    false, p_frame);

    int n_windows = 22;
    for(int i=0; i<n_windows; i++)
    {
        int row = i/win_per_row;
        int col = i%win_per_row;
        new UIFrame(settings, win_start_x+col*win_off_x, win_start_y+row*win_off_y, win_w, win_h, win_color, true, p_scrollframe);
    }

    
    p_scrollframe->total_h = 2*space-window_space + -(std::ceil(n_windows/(double)win_per_row)*win_off_y);
    p_scrollframe->total_h = std::max(frame_h, p_scrollframe->total_h);
    std::function<void(double, double)> slider_fun= std::bind(&UIScrollFrame::scroll_move_win, p_scrollframe,std::placeholders::_1, std::placeholders::_2);
    UIYSlider* p_slider = new UIYSlider(settings, space, space, tick_w, line_h, glm::vec3(54/255.0, 222/255.0, 99/255.0), 
                                       tick_h, line_w, 0, p_scrollframe->total_h-frame_h, 
                                       slider_fun, 
                                       p_frame);
    p_slider->value=p_slider->max;



}