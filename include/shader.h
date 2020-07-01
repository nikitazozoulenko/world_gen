#ifndef SHADER_WITH_ID_H
#define SHADER_WITH_ID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>
#include <unordered_map>

class Shader 
{
public:
    Shader(const std::string& filename, GLenum shadertype, std::unordered_map<std::string, std::string> variable_map={}); //to make const variables in shaders
	std::string ParseShaderFile(const std::string& filename, std::unordered_map<std::string, std::string> variable_map);
	void CheckShaderError(unsigned int flag, const std::string& errorMessage);
	unsigned int CreateShader(const std::string& shader_string, GLenum shadertype);
	unsigned int ID;
protected:
private:
};

#endif // SHADER_WITH_ID_H
