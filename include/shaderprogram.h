#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

//Library
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//Standard includes
#include <string>

//Own includes
#include <shader.h>

class Shaderprogram 
{
public:
    Shaderprogram() = default;
    Shaderprogram(const char * vertex_path, const char * geometry_path, const char * fragment_path, const char * compute_path, std::unordered_map<std::string, std::string> variable_map={});
    //example variable_map = {"MAX_OCTAVES" : "const float MAX_OCTAVES = 8"}
    void checkProgramError(unsigned int flag, const std::string& error_message);
    void setUniformMat4(const char * name, glm::mat4 matrix);
    void setUniformVec4(const char * name, glm::vec4 vector);
    void setUniformVec4(const char * name, float x, float y, float z, float w);
    void setUniformVec3(const char * name, glm::vec3 vector);
    void setUniformVec3(const char * name, float x, float y, float z);
    void setUniformVec2(const char * name, glm::vec2 vector);
    void setUniformVec2(const char * name, float x, float y);
    void setUniformFloat(const char * name, float number);
    void setUniformInt(const char * name, int number);  
    unsigned int ID;
    void bind();
    void unbind();
protected:
private:
};

#endif // SHADERPROGRAM_H
