#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>

//Own includes
#include "shader.h"

class Shaderprogram 
{
public:
    Shaderprogram(Shader& vertexShader, Shader& fragmentShader);
    void CheckProgramError(unsigned int flag, const std::string& errorMessage);
    unsigned int shaderProgramID;
protected:
private:
};

#endif // SHADERPROGRAM_H
