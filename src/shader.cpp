#include "../include/shader.h"

#include <iostream>
#include <fstream>

Shader::Shader(const std::string& filename, GLenum shadertype)
{
	std::string shader_string = ParseShaderFile(filename);
	CreateShader(shader_string, shadertype);
}


std::string Shader::ParseShaderFile(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader with file name: " << filename << std::endl;
	}
	return output;
}


unsigned int Shader::CreateShader(const std::string& shader_string, GLenum shadertype)
{
	shaderID = glCreateShader(shadertype); //GL_VERTEX_SHADER, GL_FRA...

	if (shaderID == 0) {
		std::cerr << "Error: Shader creation failed" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	int shaderSourceStringLengths[1] = { shader_string.length() };
	shaderSourceStrings[0] = shader_string.c_str();
	glShaderSource(shaderID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shaderID);

	CheckShaderError(GL_COMPILE_STATUS, "Error: Shader compilation failed");

	return shaderID;
}


void Shader::CheckShaderError(unsigned int flag, const std::string& errorMessage)
{
	int success = 0;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shaderID, flag, &success);

	if (success == GL_FALSE) 
	{
		glGetShaderInfoLog(shaderID, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}