#include "../include/shaderprogram.h"
#include "../include/misc.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>
#include <vector>



Shaderprogram::Shaderprogram(const char * vertex_path, const char * geometry_path, const char * fragment_path, const char * compute_path)
{
    std::vector<Shader> shaders;
    std::unordered_map<GLenum, const char *> paths;
    paths[GL_VERTEX_SHADER] = vertex_path;
    paths[GL_GEOMETRY_SHADER] = geometry_path;
    paths[GL_FRAGMENT_SHADER] = fragment_path;
    paths[GL_COMPUTE_SHADER] = compute_path;

    ID = glCreateProgram();
    for (auto& pair : paths)
    {
        const char * path = pair.second;
        GLenum shader_mode = pair.first;
        if (path)
        {
            Shader shader = Shader(path, shader_mode);
            shaders.push_back(shader);
            glAttachShader(ID, shader.ID);
        }
    }
    glLinkProgram(ID);
    checkProgramError(GL_LINK_STATUS, "Error: Program linking failed");

    for (Shader& shader : shaders)
        glDeleteShader(shader.ID);
}


void Shaderprogram::checkProgramError(unsigned int flag, const std::string& error_message)
{
	int success = 0;
	GLchar error[1024] = { 0 };

	glGetProgramiv(ID, flag, &success);

	if (success == GL_FALSE) 
	{
		glGetProgramInfoLog(ID, sizeof(error), NULL, error);
		std::cerr << error_message << ". Reason: " << error << std::endl;
	}
}


void Shaderprogram::bind()
{
	glUseProgram(ID);
}


void Shaderprogram::unbind()
{
	glUseProgram(0);
}


void Shaderprogram::setUniformMat4(const char * name, glm::mat4 matrix)
{
	//retrieve the matrix uniform locations
    unsigned int loc = glGetUniformLocation(ID, name);
    //pass uniforms to shader
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shaderprogram::setUniformVec3(const char * name, glm::vec3 vector)
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