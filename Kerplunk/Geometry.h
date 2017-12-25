#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <map>
#include "IcoSphere.h"
//#include <GLFW/glfw3.h>

class Geometry
{
public:
	// Stores all the vertex data associated with the object. Order = Position, Normal, texcoord
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int numberOfTriangles = 0;
	unsigned int VAO_Handle = 0;
	unsigned int VBO_Handle = 0;
	unsigned int EBO_Handle = 0;

	enum primitiveTypes {
		icoSphere,
	};
	static std::string ToString(primitiveTypes type);

	// Create a geometry oject loading in vertex data from a file.
	Geometry();

	void LoadGeometry(std::string pFilePath);
	void LoadGeometry(primitiveTypes type);

	void render();
	void renderElements();

	~Geometry();

private:
	void BufferData();

	void BufferElementData();
	void generateIcoSphere();
};
#endif