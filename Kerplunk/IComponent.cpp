#include "IComponent.h"

IComponent::IComponent(ComponentFlags componentType) : componentType(componentType) {}

IComponent::~IComponent()
{
}
