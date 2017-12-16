#ifndef COMPONENTROTATION_H
#define COMPONENTROTATION_H

#include "IComponent.h"
#include <glm/glm.hpp>

class ComponentRotation : public IComponent
{
public:
	glm::vec3 rotation;

	ComponentRotation();
	ComponentRotation(float x, float y, float z);
	ComponentRotation(glm::vec3 pos);

	~ComponentRotation();
};
#endif