#include "IComponent.h"

IComponent::IComponent(ComponentFlags componentType) : componentType(componentType) {}

IComponent::ComponentFlags IComponent::GetComponentType()
{
	return componentType;
}

IComponent::~IComponent()
{
}
