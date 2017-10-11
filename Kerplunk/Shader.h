//#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	// Reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	// Activates the shader
	void use();

	// Utility uniform functions

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	void setMatrix4(const std::string & name, float *value) const;

	~Shader();

	// The program ID
	unsigned int ID;


};

#endif