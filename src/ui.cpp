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
    UIWindow* p_window = createUIWindow(0.1, 0.1, 0.3, 0.2, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    std::vector<UISlider>& sliders = p_window->sliders;

    //how big a slider is
    double w = 0.44;
    double off_x = (1-1*w)/2.0;
    double h = 0.25;
    double off_y = (1-2*h)/3.0;

    double tick_w = 0.3;
    double line_h = 0.2;

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
    UIWindow* p_window = createUIWindow(0.1, 0.1, 0.6, 0.4, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    std::vector<UISlider>& sliders = p_window->sliders;

    //how big a slider is
    double w = 0.44;
    double off_x = (1-2*w)/3.0;
    double h = 0.25;
    double off_y = (1-3*h)/4.0;

    double tick_w = 0.3;
    double line_h = 0.2;

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
