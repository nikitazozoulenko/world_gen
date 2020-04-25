#include <settings.h>
#include <misc.h>

Settings::Settings() :
    width(1280),
    height(720),
    window_title("Marching Cubes"),
    chunk_width(40),
    chunk_height(40),
    chunk_depth(40)
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

int Settings::getChunkWidth()
{
    return chunk_width;
}

int Settings::getChunkHeight()
{
    return chunk_height;
}

int Settings::getChunkDepth()
{
    return chunk_depth;
}