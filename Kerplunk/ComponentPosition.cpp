#include "ComponentPosition.h"

ComponentPosition::ComponentPosition() 
{
	position = glm::vec3(0);
}

ComponentPosition::ComponentPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

ComponentPosition::ComponentPosition(glm::vec3 pos)
{
	position = pos;
}

ComponentPosition::~ComponentPosition() {}