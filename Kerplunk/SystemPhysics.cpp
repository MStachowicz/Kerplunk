#include "SystemPhysics.h"

SystemPhysics::SystemPhysics() : ISystem("SystemPhysics", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_VELOCITY))
{}

// Applies the velocity to all the entities 
void SystemPhysics::OnLoad(Entity & entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		std::shared_ptr<ComponentVelocity> velComp = std::dynamic_pointer_cast<ComponentVelocity> (entity.FindComponent(16));

		// If the positions are the same (no velocity, apply the velocity component)
		if (posComp->lastPosition == posComp->position)
			posComp->lastPosition -= velComp->Velocity;
	}
}

void SystemPhysics::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		//std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));

		std::shared_ptr<ComponentVelocity> velComp =
			std::dynamic_pointer_cast<ComponentVelocity> (entity.FindComponent(16));

		Motion(posComp->position, posComp->lastPosition, 0.5f);
	}

	// Check every entity with collision component for collisions in previous run.
	if (((entity.mask & IComponent::COMPONENT_COLLISION) == IComponent::COMPONENT_COLLISION))
	{
		std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (entity.FindComponent(32768));

		// Perform the collision response and remove the collision from the vector of collisions from the end down.
		for (unsigned int i = collisionComp->collisions.size(); i-- > 0;)
		{
			std::shared_ptr<ComponentCollision> CollidedCollisionComp = std::dynamic_pointer_cast<ComponentCollision>(collisionComp->collisions[i].entityCollidedWith.FindComponent(32768));

			switch (collisionComp->type)
			{
			case ComponentCollision::collisionPrimitiveType::Sphere:

				switch (CollidedCollisionComp->type)
				{
				case ComponentCollision::collisionPrimitiveType::Plane: // Sphere on plane collision response
					spherePlaneResponse(entity, collisionComp->collisions[i].entityCollidedWith, collisionComp->collisions[i]);
					break;
				}

				break;
			}

			//collisionComp->collisions.erase(collisionComp->collisions.begin() + i); // this method requires writing the gang of three in the collisionData struct
			collisionComp->collisions.pop_back();
		}
	}
}


// Performs the physics calculations to apply motion through the verlet integration method.
void SystemPhysics::Motion(glm::vec3 &pPosition, glm::vec3 &pLastPosition, float deltaTime)
{
	// Calculating the velocity
	glm::vec3 velocity = glm::vec3(pPosition - pLastPosition);
	// Setting the previous position as the current position.
	pLastPosition = pPosition;
	// Apply the velocity
	pPosition += velocity;


	//pPosition += pVelocity * deltaTime;
}

SystemPhysics::~SystemPhysics() {}

void SystemPhysics::spherePlaneResponse(Entity &pSphere, Entity &pPlane, ComponentCollision::CollisionData &pCollisionData)
{
	std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (pSphere.FindComponent(2));
	std::shared_ptr<ComponentVelocity> velComp = std::dynamic_pointer_cast<ComponentVelocity> (pSphere.FindComponent(16));

	posComp->lastPosition = posComp->lastPosition;// pCollisionData.collisionPoint;

}
