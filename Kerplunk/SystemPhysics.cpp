#include "SystemPhysics.h"

SystemPhysics::SystemPhysics() : ISystem("SystemPhysics", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_VELOCITY))
{}

void SystemPhysics::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		//std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		
		std::shared_ptr<ComponentVelocity> velComp =
			std::dynamic_pointer_cast<ComponentVelocity> (entity.FindComponent(16));

		Motion(posComp->position, velComp->Velocity, 0.5f);
	}

	// Check every entity with collision component for collisions in previous run.
	if (((entity.mask & IComponent::COMPONENT_COLLISION) == IComponent::COMPONENT_COLLISION))
	{
		std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (entity.FindComponent(32768));

		// Perform the collision response and remove the collision from the vector of collisions.
		for (unsigned int i = 0; i < collisionComp->collisions.size(); i++)
		{
			collisionComp->collisions.erase(collisionComp->collisions.begin() +  i);
		}
	}
}

void SystemPhysics::Motion(glm::vec3 &pPosition, glm::vec3 pVelocity, float deltaTime)
{
	pPosition += pVelocity * deltaTime;
}

SystemPhysics::~SystemPhysics() {}
