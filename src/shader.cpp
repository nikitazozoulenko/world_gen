#include "../include/shader.h"

#include <iostream>
#include <fstream>
#include <regex>

Shader::Shader(const std::string& filename, GLenum shadertype, std::unordered_map<std::string, std::string> variable_map)
{
	std::string shader_string = ParseShaderFile(filename, variable_map);
	CreateShader(shader_string, shadertype);
}


std::string Shader::ParseShaderFile(const std::string& filename, std::unordered_map<std::string, std::string> variable_map)
{
	std::ifstream file;
	file.open(filename);

	std::string output;
	std::string line;

	if (file.is_open()) 
	{
		while (file.good()) 
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else 
	{
		std::cerr << "Unable to load shader with file name: " << filename << std::endl;
	}

	//replace all createvar with real variable
	for(auto& pair : variable_map)
	{
		output = std::regex_replace(output, std::regex("createvar "+pair.first), pair.second);
	}
	return output;
}


unsigned int Shader::CreateShader(const std::string& shader_string, GLenum shadertype)
{
	ID = glCreateShader(shadertype); //GL_VERTEX_SHADER, GL_FRA...
	if (ID == 0) 
		std::cerr << "Error: Shader creation failed" << std::endl;

	const GLchar* shaderSourceStrings[1];
	int shaderSourceStringLengths[1] = { (int) shader_string.length() };
	shaderSourceStrings[0] = shader_string.c_str();
	glShaderSource(ID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(ID);

	CheckShaderError(GL_COMPILE_STATUS, "Error: Shader compilation failed");

	return ID;
}


void Shader::CheckShaderError(unsigned int flag, const std::string& errorMessage)
{
	int success = 0;
	GLchar error[1024] = { 0 };

	glGetShaderiv(ID, flag, &success);

	if (success == GL_FALSE) 
	{
		glGetShaderInfoLog(ID, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}