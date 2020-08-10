#ifndef MY_SETTINGS_THING_H
#define MY_SETTINGS_THING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>




// Stores all settings that you would want to have
class Settings 
{
public:
    Settings();
    int getWindowHeight();
    int getWindowWidth();
    glm::vec2 getContextCreationDimensions();
    std::string getWindowTitle();
    int getChunkWidth();
    int getChunkHeight();
    int getChunkDepth();
    int getRenderDistance();

    int MAX_OCTAVES;
    int reach;

    float zNear;
    float zFar;
protected:
private:
    int width;
    int height;
    std::string window_title;

    int chunk_width;
    int chunk_height;
    int chunk_depth;

    int render_distance_chunks;
};

#endif // MY_SETTINGS_THING_H
