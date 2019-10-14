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
protected:
private:
    int width;
    int height;
    std::string window_title;
};

#endif // MY_SETTINGS_THING_H
