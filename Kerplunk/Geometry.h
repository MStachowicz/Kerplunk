#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>

class Geometry
{
public:
	// Stores all the vertex data associated with the object. Order = Position, Normal, Texture coord
	std::vector<float> vertices;
	unsigned int numberOfTriangles;
	unsigned int VAO_Handle;
	unsigned int VBO_Handle;

	// Create a geometry oject loading in vertex data from a file.
	Geometry();

	void LoadGeometry(std::string pFilePath);

	void render();

	~Geometry();

private:
	void BufferData();

};
#endif