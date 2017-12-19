#ifndef COMPONENTGEOMETRY_H
#define COMPONENTGEOMETRY_H

#include "IComponent.h"
#include <string>
#include <glm/glm.hpp>

class ComponentGeometry : public IComponent
{
public:
	// Create a geometry oject loading in vertex data from a file.
	ComponentGeometry(std::string filePath);
	// Create a geometry object using an array of vertex data.
	ComponentGeometry(float pVertices[]);

	~ComponentGeometry();
};
#endif