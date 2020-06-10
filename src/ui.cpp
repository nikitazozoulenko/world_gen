#include <algorithm>

#include <ui.h>
#include <scene.h>

UI::UI()
{

}

UIWindow* UI::createUIWindow(double x0, double y0, double width, double height, glm::vec3 color)
{
    //add to list
    UIWindow* p_ui_window = new UIWindow(x0, y0, width, height, color);
    windows.push_back(p_ui_window);
    return p_ui_window;
}

void UI::removeUIWindow(UIWindow* p_ui_window)
{
    //remove from list
    auto position = std::find(windows.begin(), windows.end(), p_ui_window);
    windows.erase(position);
    delete p_ui_window;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// FreeCamWorld //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UI_FreeCamWorld::UI_FreeCamWorld(FreeCamWorld* p_scene) :
    p_scene(p_scene)
{
}



void UI_FreeCamWorld::createUI()
{
    double win_w = 300;
    double win_h = 300;
    UIWindow* p_window = createUIWindow(100, 100, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    std::vector<UISlider>& sliders = p_window->sliders;

    //how big a slider is
    double w = 200;
    double off_x = (win_w-w)/2.0;
    double h = 100;
    double off_y = (win_h-2*h)/3.0;

    double tick_w = 50;
    double line_h = 40;

    double m_size = 2;
    double M_size = 100;
    double m_amp = 0;
    double M_amp = 2;
    
    std::function<void(double, double)> fun_amp = std::bind(&FreeCamWorld::slider_amp_fun, p_scene, std::placeholders::_1, std::placeholders::_2);
    std::function<void(double, double)> fun_size = std::bind(&FreeCamWorld::slider_size_fun, p_scene, std::placeholders::_1, std::placeholders::_2);

    sliders.push_back(UISlider(m_size, M_size, off_x, off_y, w, h, tick_w, line_h, fun_size));
    sliders.push_back(UISlider(m_amp, M_amp, off_x, off_y*2+h, w, h, tick_w, line_h, fun_amp));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UI_MainMenu::UI_MainMenu(MainMenu* p_scene) :
    p_scene(p_scene)
{

}

void UI_MainMenu::createUI()
{
    double win_w = 700;
    double win_h = 400;
    UIWindow* p_window = createUIWindow(200, 100, win_w, win_h, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    std::vector<UISlider>& sliders = p_window->sliders;

    //how big a slider is
    double w = 300;
    double off_x = (win_w-2*w)/3.0;
    double h = 100;
    double off_y = (win_h-3*h)/4.0;

    double tick_w = 50;
    double line_h = 20;

    double min_val = 10;
    double max_val = 20;
    
    std::function<void(double, double)> fun = std::bind(&MainMenu::test, p_scene, std::placeholders::_1, std::placeholders::_2);

    sliders.push_back(UISlider(min_val, max_val, off_x, off_y, w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y, w, h, tick_w, line_h, fun));

    sliders.push_back(UISlider(min_val, max_val, off_x, off_y*2+h, w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y*2+h, w, h, tick_w, line_h, fun));

    sliders.push_back(UISlider(min_val, max_val, off_x, off_y*3 +h+h, w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, off_x*2 + w, off_y*3 +h+h, w, h, tick_w, line_h, fun));
}
