#include <settings.h>
#include <misc.h>

Settings::Settings() :
    width(500),
    height(500),
    window_title("Marching Cubes")
{

}


int Settings::getWindowWidth()
{   
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return viewport[2];
}


glm::vec2 Settings::getContextCreationDimensions()
{   
    return glm::vec2(width, height);
}


int Settings::getWindowHeight()
{   
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return viewport[3];
}

std::string Settings::getWindowTitle()
{
    return window_title;
}