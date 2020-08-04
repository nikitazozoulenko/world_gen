#include <scene.h>
#include <misc.h>


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
    end_scene();
    if(p_input_scheme)
    {
        delete p_input_scheme;
    }
}

void Scene::end_scene()
{
    print_float("base end", 0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Free Cam Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FreeCamWorld::FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new FreeCamWorldInputScheme(settings, window, camera, this), masterRenderer),
    ui(this),
    world(settings, camera, masterRenderer.block_renderer.blockIDMap)
{
    ui.createUI();
}

FreeCamWorld::~FreeCamWorld()
{
    end_scene();
}

void FreeCamWorld::render()
{
    masterRenderer.render_freecamworld(this);
}


void FreeCamWorld::scene_logic(double delta_time)
{
    world.gameLogic(delta_time);
}

void FreeCamWorld::end_scene()
{
    print_float("end scene", 0);
    world.end();
}

void FreeCamWorld::slider_amp_fun(double val, double change)
{
    print_float("slider",0);
    // world.chunk_manager.amplitude=val;
}

void FreeCamWorld::slider_size_fun(double val, double change)
{
    print_float("slider",1);
    // world.chunk_manager.size=val;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new MainMenuInputScheme(settings, window, camera, this), masterRenderer),
    ui(this)
{
    ui.createUI();
}

MainMenu::~MainMenu()
{
    end_scene();
}

void MainMenu::test(double val, double change)
{
    print_float("sliding", 0);
}


void MainMenu::render()
{
    masterRenderer.render_mainmenu(this);
}

void MainMenu::end_scene()
{
    
}


void MainMenu::scene_logic(double delta_time)
{
    
}