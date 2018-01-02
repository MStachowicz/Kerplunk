#ifndef COMPONENTLIGHTDIRECTION_H
#define COMPONENTLIGHTDIRECTION_H

#include "IComponent.h"
#include <glm\glm.hpp>

class ComponentLightDirection : public IComponent
{
public:
	glm::vec3 direction;

	ComponentLightDirection(glm::vec3 pDirection);
	~ComponentLightDirection();
};
#endif