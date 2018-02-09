#include "SystemPhysics.h"

SystemPhysics::SystemPhysics() : ISystem("SystemPhysics", (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY))
{}

// Applies the velocity to all the entities 
void SystemPhysics::OnLoad(Entity & entity)
{
	if ((entity.mask & MASK) == MASK)
	{
	}
}

// Performs the Euler integration to find the position of the body after a timestep forward.
void SystemPhysics::UpdateBodyPosition(glm::vec3 &pPosition, glm::vec3 &pVelocity, float tickDeltaTime)
{
	pPosition = pPosition + (tickDeltaTime * pVelocity);
}

void SystemPhysics::UpdateBodyVelocity(glm::vec3 & pVelocity, glm::vec3 & resultantForce, const float & pMass, const float & tickDeltaTime)
{
	pVelocity = pVelocity + (tickDeltaTime * (resultantForce / pMass));
}

// Updates the timers associated with a physics tick
void SystemPhysics::UpdateTiming(float &pDeltaTime)
{
	if (pDeltaTime > 1) // Clamps the physics system to update the simulation by a regular timestep when the timestep between ticks is too large.
	{
		pDeltaTime = 1.0f / 60.0f;
		std::cout << "Clamped the time between the frame calls to " << pDeltaTime << std::endl;
	}
	else
	{
		// Increment the total time running and update the time since the last physics tick from the timestep passed from main
		SimulationTime += pDeltaTime;
		tickDeltaTime = SimulationTime - tickLastTime;
		tickLastTime = SimulationTime;
	}
}

void SystemPhysics::Tick(Entity &entity)
{
	//std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

	if ((entity.mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentRigidBody> RigidBodyComponent =
			std::dynamic_pointer_cast<ComponentRigidBody> (entity.FindComponent(65536));

		if (!pauseSimulation)
		{
			// LINEAR DYNAMICS
			//------------------------------------------------------------------------------------------------------
			UpdateBodyPosition(RigidBodyComponent->position, RigidBodyComponent->velocity, tickDeltaTime);

			if (applyGravity)
				RigidBodyComponent->forcesApplied.push_back(ComponentRigidBody::Force(RigidBodyComponent->position, glm::vec3(0.0f, -9.81f, 0.0f)));


			// Resolve all the linear forces applied to the body since the last physics tick updating the resultant force.
			RigidBodyComponent->ApplyForces();


			// Setting the new velocity integrated from the current velocity and acceleration through all the forces applied to the body since the last tick
			UpdateBodyVelocity(RigidBodyComponent->velocity, RigidBodyComponent->resultantForce, RigidBodyComponent->mass, tickDeltaTime);

			//// Debug position and velocity of the body at time in simulation
			//std::cout << "Velocity: " << RigidBodyComponent->velocity.x << ", " << RigidBodyComponent->velocity.y << ", " << RigidBodyComponent->velocity.z <<
			//	" Position: " << RigidBodyComponent->position.x << ", " << RigidBodyComponent->position.y << ", " << RigidBodyComponent->position.z <<
			//	" Time: " << SimulationTime << std::endl;
		}
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

SystemPhysics::~SystemPhysics() {}

void SystemPhysics::spherePlaneResponse(Entity &pSphere, Entity &pPlane, ComponentCollision::CollisionData &pCollisionData)
{
	std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (pSphere.FindComponent(2));
	std::shared_ptr<ComponentVelocity> velComp = std::dynamic_pointer_cast<ComponentVelocity> (pSphere.FindComponent(16));

	posComp->lastPosition = posComp->lastPosition;// pCollisionData.collisionPoint;

}
