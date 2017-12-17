#include "ComponentPosition.h"

ComponentPosition::ComponentPosition() : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = glm::vec3(0);
}

ComponentPosition::ComponentPosition(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = glm::vec3(x, y, z);
}

ComponentPosition::ComponentPosition(glm::vec3 pos) : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = pos;
}

ComponentPosition::~ComponentPosition() {}