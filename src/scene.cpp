#include <scene.h>

Scene::Scene(GLFWwindow* window):
    camera(Camera()),
    input_scheme(InputScheme(window, &camera))
{

}

void Scene::render(MasterRenderer& masterRenderer)
{
    masterRenderer.render(&camera);
}