#include "ComponentLightDirection.h"

ComponentLightDirection::ComponentLightDirection(glm::vec3 pDirection) : IComponent(IComponent::ComponentFlags::COMPONENT_LIGHT_DIRECTION), direction(pDirection)
{
}


ComponentLightDirection::~ComponentLightDirection()
{
}
