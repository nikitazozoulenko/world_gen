#include <algorithm>

#include <ui.h>
#include <scene.h>

UI::UI()
{

}

UIWindow* UI::createUIWindow(glm::vec2 coords, float width, float height, glm::vec3 color)
{
    //add to list
    UIWindow* p_ui_window = new UIWindow(coords, width, height, color);
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
///////////////////////////////////////////////////////////// MainMenu Scene ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UI_MainMenu::UI_MainMenu(MainMenu* p_scene) :
    p_scene(p_scene)
{

}

void UI_MainMenu::createUI()
{
    UIWindow* p_window = createUIWindow(glm::vec2(0.1, 0.1), 0.6, 0.4, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    std::vector<UISlider>& sliders = p_window->sliders;

    //how big a slider is
    float w = 0.44;
    float off_x = (1-2*w)/3.0;
    float h = 0.25;
    float off_y = (1-3*h)/4.0;

    float tick_w = 0.3;
    float line_h = 0.2;

    float min_val = 10;
    float max_val = 20;
    
    std::function<void()> fun = std::bind(&MainMenu::test, p_scene);

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y), w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y), w, h, tick_w, line_h, fun));

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y*2+h), w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y*2+h), w, h, tick_w, line_h, fun));

    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x, off_y*3 +h+h), w, h, tick_w, line_h, fun));
    sliders.push_back(UISlider(min_val, max_val, glm::vec2(off_x*2 + w, off_y*3 +h+h), w, h, tick_w, line_h, fun));


}
