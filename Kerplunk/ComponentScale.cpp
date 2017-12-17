#include "ComponentScale.h"

ComponentScale::ComponentScale() : IComponent(IComponent::ComponentFlags::COMPONENT_SCALE)
{
	scale = glm::vec3(0);
}

ComponentScale::ComponentScale(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_SCALE)
{
	scale = glm::vec3(x, y, z);
}

ComponentScale::ComponentScale(glm::vec3 pScale) : IComponent(IComponent::ComponentFlags::COMPONENT_SCALE)
{
	scale = pScale;
}

ComponentScale::~ComponentScale() {}