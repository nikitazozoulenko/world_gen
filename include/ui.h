#ifndef WORLDGEN_NIKITA_UI_H
#define WORLDGEN_NIKITA_UI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <settings.h>
#include <uiWindow.h>

class UI 
{
public:
    UI();
    std::vector<UIWindow*> windows;
    UIWindow* createUIWindow(glm::vec2 coords, float width, float height, glm::vec3 color);
    void removeUIWindow(UIWindow* p_ui_window);
protected:
private:
};

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

class MainMenu;
class UI_MainMenu: public UI
{
public:
    UI_MainMenu(MainMenu* p_scene);
    MainMenu* p_scene;
    void createUI();
protected:
private:
};


#endif // WORLDGEN_NIKITA_UI_H
