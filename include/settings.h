#ifndef MY_SETTINGS_THING_H
#define MY_SETTINGS_THING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#define SCENE_FreeCamWorld  18823
#define SCENE_MainMenu      24354

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
protected:
private:
    int width;
    int height;
    std::string window_title;

    int chunk_width;
    int chunk_height;
    int chunk_depth;
};

#endif // MY_SETTINGS_THING_H
