#include <program.h>
#include <misc.h>

#include <iostream>

Program::Program() :
    settings(Settings()),
    window(createWindow(settings)),
    masterRenderer(MasterRenderer(window, settings)),
    p_scene(createFreeCamWorld()),
    timer(Timer())
{

}

Program::~Program()
{
    if(p_scene)
    {
        delete p_scene;
    }
}


void Program::loadGLFW()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


void Program::loadGlad()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // throw error
    }    
}


GLFWwindow* Program::createWindow(Settings& settings)
{
    loadGLFW();
    // glfw window creation
    // --------------------
    glm::vec2 dimensions = settings.getContextCreationDimensions();
    std::string window_title = settings.getWindowTitle();
    GLFWwindow* window = glfwCreateWindow(dimensions.x, dimensions.y, window_title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    loadGlad();
    return window;
}


FreeCamWorld* Program::createFreeCamWorld()
{
    return new FreeCamWorld(settings, window, masterRenderer);
}


MainMenu* Program::createMainMenu()
{
    return new MainMenu(settings, window, masterRenderer);
}


void Program::changeSceneIfNeeded()
{
    int change_scene = p_scene->p_input_scheme->change_scene;
    if(change_scene)
    {
        p_scene->p_input_scheme->remove();
        delete p_scene;
        if(change_scene == SCENE_MainMenu)
            p_scene = createMainMenu();
        else if(change_scene == SCENE_FreeCamWorld)
            p_scene = createFreeCamWorld();
        p_scene->p_input_scheme->init();
    }
}


void Program::run()
{
    p_scene->p_input_scheme->init();
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        changeSceneIfNeeded();
        // per-frame time logic
        float delta_time = timer.update_delta_time();
        p_scene->p_input_scheme->processInput(delta_time);
        p_scene->scene_logic(delta_time);
        //render
        p_scene->render();
        p_scene->frame +=1;
    }
    glfwTerminate();
}