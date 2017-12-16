#include "ComponentScale.h"

ComponentScale::ComponentScale()
{
	scale = glm::vec3(0);
}

ComponentScale::ComponentScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

ComponentScale::ComponentScale(glm::vec3 pScale)
{
	scale = pScale;
}

ComponentScale::~ComponentScale() {}