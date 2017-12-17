#ifndef COMPONENTVELOCITY_H
#define COMPONENTVELOCITY_H

#include "IComponent.h"
#include <glm/glm.hpp>

class ComponentVelocity : public IComponent
{
public:
	glm::vec3 Velocity;

	ComponentVelocity();
	ComponentVelocity(float x, float y, float z);
	ComponentVelocity(glm::vec3 pVelocity);

	~ComponentVelocity();
};
#endif