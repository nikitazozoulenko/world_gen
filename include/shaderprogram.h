#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>

#include <glm/glm.hpp>

//Own includes
#include "shader.h"

class Shaderprogram 
{
public:
    Shaderprogram(const char * vertex_path, const char * fragment_path);
    void checkProgramError(unsigned int flag, const std::string& error_message);
    void setUniformMat4(const char * name, glm::mat4& matrix);
    void setUniformVec3(const char * name, glm::vec3& vector);
    void setUniformVec3(const char * name, float x, float y, float z);
    void setUniformFloat(const char * name, float number);
    void setUniformInt(const char * name, int number);  
    unsigned int ID;
    void use();
protected:
private:
};

#endif // SHADERPROGRAM_H
