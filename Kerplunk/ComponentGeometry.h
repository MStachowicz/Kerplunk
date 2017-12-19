#ifndef COMPONENTGEOMETRY_H
#define COMPONENTGEOMETRY_H

#include <string>
#include <glm/glm.hpp>
#include <memory>
#include "IComponent.h"
#include "ResourceManager.h"
#include "Geometry.h"

class ComponentGeometry : public IComponent
{
public:
	std::shared_ptr<Geometry> geometry;

	// Create a geometry object loading in vertex data from a file using the resource manager.
	ComponentGeometry(std::string filePath);

	~ComponentGeometry();
};
#endif