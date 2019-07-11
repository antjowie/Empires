#include "Shader.h"
#include <GLES2/gl2.h>

#include <fstream>
#include <sstream>
#include <iostream>

bool Shader::addShader(unsigned shaderType, const char * shaderPath)
{
	// Temporarily create shader program here
	std::ifstream file;

	file.open(shaderPath);
	if (!file.is_open())
	{
		std::cout << "Could not find shader located at\"" << shaderPath << "\"\n";
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	const std::string string = buffer.str();
	const char* source = string.c_str();

	// Assume we always load 1 shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	// Check compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_INVALID_ENUM)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
		char info[status];
		glGetShaderInfoLog(shader, status, 0, info);
		printf("Error while compiling shader: %s\n", info);

		glDeleteShader(shader);
		return false;
	}

	m_shaders.push_back(shader);
	return true;
}

bool Shader::linkProgram(unsigned program)
{
	for (unsigned shader : m_shaders)
	{
		// Attach shader to program
		glAttachShader(program, shader);
	}

	// Check linking status
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status);
		char info[status];
		glGetProgramInfoLog(program, status, 0, info);
		printf("Error while linking program: %s\n", info);

		return false;
	}
	return true;
}

Shader::~Shader()
{
	for (unsigned shader : m_shaders)
	{
		glDeleteShader(shader);
	}
}