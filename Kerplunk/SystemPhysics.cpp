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

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		
		std::shared_ptr<ComponentVelocity> velComp =
			std::dynamic_pointer_cast<ComponentVelocity> (entity.FindComponent(16));

		Motion(posComp->position, velComp->Velocity, 0.5f);
	}
}

void SystemPhysics::Motion(glm::vec3 &pPosition, glm::vec3 pVelocity, float deltaTime)
{
	pPosition += pVelocity * deltaTime;
}

SystemPhysics::~SystemPhysics() {}
