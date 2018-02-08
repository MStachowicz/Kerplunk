#include "ComponentRigidBody.h"

ComponentRigidBody::ComponentRigidBody() : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY), mass(1), velocity(glm::vec3(0.0f,0.0f, 0.0f))
{
	position = glm::vec3(0);
}

ComponentRigidBody::ComponentRigidBody(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY), mass(1), velocity(glm::vec3(0.0f, 0.0f, 0.0f))
{
	position = glm::vec3(x, y, z);
}

ComponentRigidBody::ComponentRigidBody(glm::vec3 pos) : IComponent(IComponent::ComponentFlags::COMPONENT_RIGID_BODY), mass(1), velocity(glm::vec3(0.0f, 0.0f, 0.0f))
{
	position = pos;
}

ComponentRigidBody::~ComponentRigidBody() {}