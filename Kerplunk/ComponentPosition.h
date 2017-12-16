#ifndef COMPONENTPOSITION_H
#define COMPONENTPOSITION_H

#include "IComponent.h"
#include <glm/glm.hpp>

class ComponentPosition : public IComponent
{
public:
	glm::vec3 position;

	ComponentPosition();
	ComponentPosition(float x, float y, float z);
	ComponentPosition(glm::vec3 pos);

	~ComponentPosition();
};
#endif