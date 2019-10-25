#include <scene.h>
#include <misc.h>

//////////// SCENE ///////////////////////////////////////////////////////////

Scene::Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme):
    camera(Camera()),
    settings(settings),
    p_input_scheme(p_input_scheme)
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

FreeCamWorld::FreeCamWorld(Settings& settings, GLFWwindow* window):
    Scene(settings, window, new FreeCamWorldInputScheme(settings, window, camera)),
    world(World(settings))
{

}


void FreeCamWorld::render(MasterRenderer& masterRenderer)
{
    masterRenderer.render(world, camera);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// MainMenu Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu(Settings& settings, GLFWwindow* window):
    Scene(settings, window, new MainMenuInputScheme(settings, window, camera))
{

}

void MainMenu::render(MasterRenderer& masterRenderer)
{
    print_float("rendering", 0);
    masterRenderer.render_menu(camera);
}

