#include "SystemPhysics.h"

SystemPhysics::SystemPhysics() : ISystem("SystemPhysics", (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY)), 
applyGravity(true)//, timeScaler(TIMESCALER)
{
	// Set defaults
	timeScaler = TIMESCALER;
}
SystemPhysics::~SystemPhysics() {}

// Applies the velocity to all the entities 
void SystemPhysics::OnLoad(const std::shared_ptr<Entity> &entity)
{
	if ((entity->mask & MASK) == MASK)
	{
	}
}
void SystemPhysics::OnTickStart()
{
}
void SystemPhysics::OnTickStart(const std::shared_ptr<Entity>& entity) 
{}
void SystemPhysics::Tick(const std::shared_ptr<Entity> &entity)
{
	//std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

	if ((entity->mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentRigidBody> RigidBodyComponent =
			std::dynamic_pointer_cast<ComponentRigidBody> (entity->FindComponent(65536));

		if (!pauseSimulation)
		{
			// LINEAR DYNAMICS
			//------------------------------------------------------------------------------------------------------
			UpdateBodyPosition(RigidBodyComponent->position, RigidBodyComponent->velocity, tickDeltaTime * timeScaler);

			if (applyGravity)
				if (!RigidBodyComponent->ignorePhysics)
					ApplyGravity(RigidBodyComponent->velocity, tickDeltaTime * timeScaler);

			// Resolve all the linear forces applied to the body since the last physics tick updating the resultant force.
			RigidBodyComponent->ApplyForces();


			// Setting the new velocity integrated from the current velocity and acceleration through all the forces applied to the body since the last tick
			UpdateBodyVelocity(RigidBodyComponent->velocity, RigidBodyComponent->resultantForce, RigidBodyComponent->mass, tickDeltaTime * timeScaler);

			//// Debug position and velocity of the body at time in simulation
			//std::cout << "Velocity: " << RigidBodyComponent->velocity.x << ", " << RigidBodyComponent->velocity.y << ", " << RigidBodyComponent->velocity.z <<
			//	" Position: " << RigidBodyComponent->position.x << ", " << RigidBodyComponent->position.y << ", " << RigidBodyComponent->position.z <<
			//	" Time: " << SimulationTime << std::endl;
		}
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
void SystemPhysics::ApplyGravity(glm::vec3& pVelocity, const float& tickDeltaTime)
{
	pVelocity = pVelocity + (tickDeltaTime * glm::vec3(0.0f, -9.81f, 0.0f));
}

// Updates the timers associated with a physics tick
void SystemPhysics::UpdateTiming(float &pDeltaTime)
{
	if (pDeltaTime > 1) // Clamps the physics system to update the simulation by a regular timestep when the timestep between ticks is too large.
	{
		pDeltaTime = 1.0f / 60.0f;
		std::cout << "Clamped the time between the frame calls to " << pDeltaTime << std::endl;
	}

	// Increment the total time running and update the time since the last physics tick from the timestep passed from main
	SimulationTime += pDeltaTime;
	tickDeltaTime = SimulationTime - tickLastTime;
	tickLastTime = SimulationTime;

}