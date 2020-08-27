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
    std::vector<UIElement*> elements; //active elements
    std::vector<UIElement*> elements_on_cursor; //actives most inner children under cursor
    void mouse_click(double x,double y);
    void mouse_release(double x, double y);
    void process_movement(double xoff, double yoff, double x, double y);
    void update_elements_on_cursor(double x, double y);
    static std::vector<UIElement*> find_elements_on_cursor(std::vector<UIElement*>& ui_windows, double x, double y);
    static UIElement* find_top_element_on_cursor(std::vector<UIElement*>& ui_windows, double x, double y);
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
    void create_itempick();
    void create_escapemenu();

    void toggle_menu();
    void toggle_inventory();
    void toggle_element(UIElement* p_ele);

    bool is_cursor_mode();

    UIElement* p_itempick_base;
    UIElement* p_escapemenu;
    UIElement* p_cursor;
protected:
private:
};



class Editor;

class UI_Editor: public UI
{
public:
    UI_Editor(Settings& settings, Editor* p_scene);
    ~UI_Editor();
    void create_ui();

    int win_per_row;
protected:
private:
};


#endif // WORLDGEN_NIKITA_UI_H
