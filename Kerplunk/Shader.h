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
	
	~Shader();

	// The program ID
	unsigned int ID;


};

#endif