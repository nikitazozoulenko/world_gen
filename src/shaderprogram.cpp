#include "../include/shaderprogram.h"

#include <iostream>


Shaderprogram::Shaderprogram(Shader& vertexShader, Shader& fragmentShader)
{
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader.shaderID);
    glAttachShader(shaderProgramID, fragmentShader.shaderID);
    glLinkProgram(shaderProgramID);

    CheckProgramError(GL_LINK_STATUS, "Error: Program linking (?) failed");
}


void Shaderprogram::CheckProgramError(unsigned int flag, const std::string& errorMessage)
{
	int success = 0;
	GLchar error[1024] = { 0 };

	glGetProgramiv(shaderProgramID, flag, &success);

	if (success == GL_FALSE) 
	{
		glGetProgramInfoLog(shaderProgramID, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}


void Shaderprogram::Use()
{
	glUseProgram(shaderProgramID);
}