#include <scene.h>

//////////// SCENE ///////////////////////////////////////////////////////////

Scene::Scene(Settings* p_settings, GLFWwindow* window, InputScheme* p_input_scheme):
    camera(Camera()),
    p_settings(p_settings),
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

//////////// FREE CAM SCENE ///////////////////////////////////////////////////////////

FreeCamWorld::FreeCamWorld(Settings* p_settings, GLFWwindow* window):
    Scene(p_settings, window, new FreeCamWorldInputScheme(window, &camera)),
    world(World(p_settings))
{

}


void FreeCamWorld::render(MasterRenderer& masterRenderer)
{
    masterRenderer.render(world, camera);
}
