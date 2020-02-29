#include <scene.h>
#include <misc.h>

#include <algorithm>

//////////// SCENE ///////////////////////////////////////////////////////////

Scene::Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme, MasterRenderer& masterRenderer):
    camera(Camera()),
    settings(settings),
    p_input_scheme(p_input_scheme),
    masterRenderer(masterRenderer)
{

}

Scene::~Scene()
{
    if(p_input_scheme)
    {
        delete p_input_scheme;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Free Cam Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FreeCamWorld::FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new FreeCamWorldInputScheme(settings, window, camera), masterRenderer),
    world(World(settings))
{

}


void FreeCamWorld::render()
{
    masterRenderer.render_freecamworld(world, camera);
}


void FreeCamWorld::scene_logic(float delta_time)
{
    
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new MainMenuInputScheme(settings, window, camera, ui_windows), masterRenderer)
{
    createUI();
}


void MainMenu::createUI()
{   
    createUIWindow(glm::vec2(0.1, 0.1), 0.4, 0.4, glm::vec3(54/255.0, 54/255.0, 99/255.0));
    createUIWindow(glm::vec2(0.2, 0.6), 0.7, 0.3, glm::vec3(0.8,0.3,0.2));
}


void MainMenu::createUIWindow(glm::vec2 coords, float width, float height, glm::vec3 color)
{
    //add to list
    UIWindow* p_ui_window = new UIWindow(coords, width, height, color);
    ui_windows.push_back(p_ui_window);
    //add render info
    masterRenderer.ui_renderer.add_ui_window(p_ui_window);
}


void MainMenu::removeUIWindow(UIWindow* p_ui_window)
{
    //remove all render info
    masterRenderer.ui_renderer.remove_ui_window(p_ui_window);
    //remove from list
    auto position = std::find(ui_windows.begin(), ui_windows.end(), p_ui_window);
    ui_windows.erase(position);

}


void MainMenu::render()
{
    masterRenderer.render_mainmenu(ui_windows, camera);
}


void MainMenu::scene_logic(float delta_time)
{
    
}