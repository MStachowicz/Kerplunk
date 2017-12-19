#ifndef COMPONENTGEOMETRY_H
#define COMPONENTGEOMETRY_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "IComponent.h"

class ComponentGeometry : public IComponent
{
public:
	// Stores all the vertex data associated with the object. Order = Position, Normal, Texture coord
	std::vector<float> vertices;
	unsigned int numberOfTriangles;
	unsigned int VAO_Handle;
	unsigned int VBO_Handle;

	// Create a geometry oject loading in vertex data from a file.
	ComponentGeometry(std::string filePath);
	// Create a geometry object using an array of vertex data.
	ComponentGeometry(std::vector<float> pVertices);

	void render();

	~ComponentGeometry();

private:
	void BufferData();

};
#endif