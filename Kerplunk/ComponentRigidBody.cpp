#include "ComponentRigidBody.h"

ComponentRigidBody::ComponentRigidBody() : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY),  
mass(DEFAULTMASS), scale(DEFAULTSCALE), velocity(DEFAULTVELOCITY)
{
	position = glm::vec3(0);
}

ComponentRigidBody::ComponentRigidBody(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY), 
mass(DEFAULTMASS), scale(DEFAULTSCALE), velocity(DEFAULTVELOCITY)
{
	position = glm::vec3(x, y, z);
}

ComponentRigidBody::ComponentRigidBody(glm::vec3 pos) : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY), 
mass(DEFAULTMASS), scale(DEFAULTSCALE), velocity(DEFAULTVELOCITY)
{
	position = pos;
}

// Updates the resultant force of the body summing all the forces being applied to it.
void ComponentRigidBody::ApplyForces()
{
	// Reset the current force on the object to zero
	resultantForce = glm::vec3(0.0f);

	// Apply all the forces pending and remove them from the list.
	for (unsigned int i = forcesApplied.size(); i-- > 0;)
	{
		resultantForce += forcesApplied[i].force;
		forcesApplied.pop_back();
	}
}

ComponentRigidBody::~ComponentRigidBody() {}