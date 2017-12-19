#include "SystemPhysics.h"
#include <iostream>
#include <vector>
#include <algorithm>

SystemPhysics::SystemPhysics() : ISystem("SystemPhysics", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_VELOCITY))
{}

void SystemPhysics::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> derived2 =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		
		std::cout << derived2->position.x << std::endl;
	}
}

void SystemPhysics::Motion(glm::vec3 &pPosition, float deltaTime)
{

}

SystemPhysics::~SystemPhysics() {}
