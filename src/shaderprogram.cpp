#include "../include/shaderprogram.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Shaderprogram::Shaderprogram(const char * vertex_path, const char * fragment_path)
{
	Shader vertex_shader = Shader(vertex_path, GL_VERTEX_SHADER);
    Shader fragment_shader = Shader(fragment_path, GL_FRAGMENT_SHADER);
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader.ID);
    glAttachShader(ID, fragment_shader.ID);
    glLinkProgram(ID);

    checkProgramError(GL_LINK_STATUS, "Error: Program linking (?) failed");

	glDeleteShader(vertex_shader.ID);
    glDeleteShader(fragment_shader.ID);
}


void Shaderprogram::checkProgramError(unsigned int flag, const std::string& error_message)
{
	int success = 0;
	GLchar error[1024] = { 0 };

	glGetProgramiv(ID, flag, &success);

	if (success == GL_FALSE) 
	{
		glGetProgramInfoLog(ID, sizeof(error), NULL, error);
		std::cerr << error_message << ": '" << error << "'" << std::endl;
	}
}


void Shaderprogram::use()
{
	glUseProgram(ID);
}


void Shaderprogram::setUniformMat4(const char * name, glm::mat4& matrix)
{
	//retrieve the matrix uniform locations
    unsigned int loc = glGetUniformLocation(ID, name);
    //pass uniforms to shader
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shaderprogram::setUniformVec3(const char * name, glm::vec3& vector)
{
    unsigned int loc = glGetUniformLocation(ID, name);
    glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void Shaderprogram::setUniformVec3(const char * name, float x, float y, float z)
{   
    unsigned int loc = glGetUniformLocation(ID, name);
    glUniform3f(loc, x, y, z);
}

void Shaderprogram::setUniformFloat(const char * name, float number)
{
    unsigned int loc = glGetUniformLocation(ID, name);
    glUniform1f(loc, number);
}

void Shaderprogram::setUniformInt(const char * name, int number)
{
    unsigned int loc = glGetUniformLocation(ID, name);
    glUniform1i(loc, number);
}