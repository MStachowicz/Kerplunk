#include "ComponentRotation.h"

ComponentRotation::ComponentRotation() : IComponent(IComponent::ComponentFlags::COMPONENT_ROTATION)
{
	rotation = glm::vec3(0);
}

ComponentRotation::ComponentRotation(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_ROTATION)
{
	rotation = glm::vec3(x, y, z);
}

ComponentRotation::ComponentRotation(glm::vec3 pScale) : IComponent(IComponent::ComponentFlags::COMPONENT_ROTATION)
{
	rotation = pScale;
}

ComponentRotation::~ComponentRotation() {}