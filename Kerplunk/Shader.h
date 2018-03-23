//#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
//#include <vector>

class Shader
{
public:
	// Reads and builds the shader
	//Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader();
	Shader(const char * vertexPath, const char * fragmentPath, const char* geometryPath);
	
	// Checks shader for comppilation or linking errors.
	void checkCompileErrors(GLuint shader, std::string type);


	// Activates the shader
	void use();

	// Utility uniform functions

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);

	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

	~Shader();

	// The program ID
	unsigned int ID;
};

#endif