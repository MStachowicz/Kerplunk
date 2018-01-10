#include "ComponentCollision.h"



ComponentCollision::ComponentCollision(collisionPrimitiveType pType) : IComponent(IComponent::ComponentFlags::COMPONENT_COLLISION), type(pType)
{
}


ComponentCollision::~ComponentCollision()
{
}
