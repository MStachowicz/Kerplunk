#include "ComponentVelocity.h"

ComponentVelocity::ComponentVelocity() : IComponent(IComponent::ComponentFlags::COMPONENT_VELOCITY)
{
	Velocity = glm::vec3(0);
}

ComponentVelocity::ComponentVelocity(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_VELOCITY)
{
	Velocity = glm::vec3(x, y, z);
}

ComponentVelocity::ComponentVelocity(glm::vec3 pVelocity) : IComponent(IComponent::ComponentFlags::COMPONENT_VELOCITY)
{
	Velocity = pVelocity;
}

ComponentVelocity::~ComponentVelocity() {}