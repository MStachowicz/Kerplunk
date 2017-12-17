#include "SystemPhysics.h"
#include <iostream>

SystemPhysics::SystemPhysics()  : ISystem("SystemPhysics",(IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_VELOCITY))
{}

void SystemPhysics::OnAction(Entity &entity) 
{
	if ((entity.mask & MASK) == MASK)
	{
		std::cout << "System: " << name << " Performing action on entity: " <<  entity.name << std::endl;

	}
} 

SystemPhysics::~SystemPhysics() {}
