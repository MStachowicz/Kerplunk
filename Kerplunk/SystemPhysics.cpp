#include "SystemPhysics.h"

SystemPhysics::SystemPhysics()  : ISystem("SystemPhysics",(IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_VELOCITY))
{}

void SystemPhysics::OnAction(Entity &entity) {} //if ((entity.mask & MASK) == MASK) }

SystemPhysics::~SystemPhysics() {}
