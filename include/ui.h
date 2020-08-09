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
    virtual void create_ui()=0;

    UIElement* p_clicked_ele; //smallest element, "inner most"
    Scene* p_scene;
protected:
private:
};



class MainMenu;

class UI_MainMenu: public UI
{
public:
    UI_MainMenu(Settings& settings, MainMenu* p_scene);
    ~UI_MainMenu();
    void create_ui();
protected:
private:
};



class FreeCamWorld;

class UI_FreeCamWorld: public UI
{
public:
    UI_FreeCamWorld(Settings& settings, FreeCamWorld* p_scene);
    ~UI_FreeCamWorld();
    void create_ui();
protected:
private:
};


#endif // WORLDGEN_NIKITA_UI_H
