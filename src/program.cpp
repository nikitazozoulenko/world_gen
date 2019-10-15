#include <program.h>
#include <misc.h>

#include <iostream>

Program::Program() :
    settings(Settings()),
    window(createWindow(settings)),
    renderer(MasterRenderer(window, &settings)),
    scene(Scene(window)),
    timer(Timer())
{

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


void Program::run()
{
    scene.input_scheme.init();
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float delta_time = timer.update_delta_time();
        scene.input_scheme.processKeyboardInput(delta_time);
        glfwPollEvents(); // to be included in input_scheme
        
        //render
        scene.render(renderer);
    }
}