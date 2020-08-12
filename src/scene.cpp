#include <scene.h>
#include <misc.h>


//////////// SCENE ///////////////////////////////////////////////////////////

Scene::Scene(Settings& settings, GLFWwindow* window, InputScheme* p_input_scheme, MasterRenderer& masterRenderer, UI* p_ui):
    camera(),
    settings(settings),
    p_input_scheme(p_input_scheme),
    masterRenderer(masterRenderer),
    p_ui(p_ui)
{
    slider_functions["test"] = std::bind(&Scene::test_slider_fun, this, std::placeholders::_1, std::placeholders::_2);
    button_functions["freecamworld"] = std::bind(&Scene::change_scene_freecamworld, this);
    button_functions["mainmenu"] = std::bind(&Scene::change_scene_mainmenu, this);
    button_functions["editor"] = std::bind(&Scene::change_scene_editor, this);
    button_functions["exit"] = std::bind(&Scene::exit_game, this);
}

Scene::~Scene()
{
    end_scene();
    if(p_input_scheme)
        delete p_input_scheme;
    if(p_ui)
        delete p_ui;
}

void Scene::end_scene()
{
    print_float("base end", 0);
}


void Scene::test_slider_fun(double val, double change)
{
    print_float("sliiiiiide scenetestfun", 0);
}

void Scene::change_scene_freecamworld()
{
    change_scene=SCENE_FreeCamWorld;
}
void Scene::change_scene_mainmenu()
{
    change_scene=SCENE_MainMenu;
}
void Scene::change_scene_editor()
{
    change_scene=SCENE_Editor;
}
void Scene::exit_game()
{
    change_scene=SCENE_ExitGame;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Free Cam Scene ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FreeCamWorld::FreeCamWorld(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new FreeCamWorldInputScheme(settings, window, camera, this), masterRenderer, new UI_FreeCamWorld(settings, this)),
    world(settings, camera, masterRenderer.block_renderer.blockIDMap)
{
    p_ui->create_ui();
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
    Scene(settings, window, new MainMenuInputScheme(settings, window, camera, this), masterRenderer, new UI_MainMenu(settings, this))
{
    p_ui->create_ui();
}

MainMenu::~MainMenu()
{
    end_scene();
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
    camera.ProcessMouseMovement(10*delta_time, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////// Editor //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Editor::Editor(Settings& settings, GLFWwindow* window, MasterRenderer& masterRenderer):
    Scene(settings, window, new EditorInputScheme(settings, window, camera, this), masterRenderer, new UI_Editor(settings, this))
{
    p_ui->create_ui();
}

Editor::~Editor()
{
    end_scene();
}


void Editor::render()
{
    masterRenderer.render_editor(this);
}

void Editor::end_scene()
{
}


void Editor::scene_logic(double delta_time)
{
    camera.ProcessMouseMovement(10*delta_time, 0);
}