#ifndef WORLDGEN_NIKITA_UI_H
#define WORLDGEN_NIKITA_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <settings.h>
#include <uiElement.h>

class Scene;//forward

class UI 
{
public:
    UI(Settings& settings, Scene* p_scene);
    virtual ~UI();
    Settings& settings;
    std::vector<UIElement*> elements; //need pointer for identification (or do i??)
    void mouse_click(double x,double y);
    void mouse_release();
    void process_movement(double xoff, double yoff, double x, double y);
    static UIElement* find_element_on_cursor(std::vector<UIElement*>& ui_windows, double x, double y);
    void create_ui();

    UIElement* p_clicked_ele; //smallest element, "inner most"
    Scene* p_scene;
protected:
private:
};



// class MainMenu;
// class UI_MainMenu: public UI
// {
// public:
//     UI_MainMenu(MainMenu* p_scene);
//     MainMenu* p_scene;
//     void createUI();
// protected:
// private:
// };



// class FreeCamWorld;
// class UI_FreeCamWorld: public UI
// {
// public:
//     UI_FreeCamWorld(FreeCamWorld* p_scene);
//     FreeCamWorld* p_scene;
//     void createUI();
// protected:
// private:
// };


#endif // WORLDGEN_NIKITA_UI_H
