#ifndef SHADER_WITH_ID_H
#define SHADER_WITH_ID_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Standard includes
#include <string>

class Shader 
{
public:
    Shader(const std::string& filename, GLenum shadertype);
	std::string ParseShaderFile(const std::string& filename);
	void CheckShaderError(unsigned int flag, const std::string& errorMessage);
	unsigned int CreateShader(const std::string& shader_string, GLenum shadertype);
	unsigned int shaderID;
protected:
private:
};

#endif // SHADER_WITH_ID_H
