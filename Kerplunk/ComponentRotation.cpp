#include "ComponentRotation.h"

ComponentRotation::ComponentRotation()
{
	rotation = glm::vec3(0);
}

ComponentRotation::ComponentRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);
}

ComponentRotation::ComponentRotation(glm::vec3 pScale)
{
	rotation = pScale;
}

ComponentRotation::~ComponentRotation() {}