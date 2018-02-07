#include "ComponentRigidBody.h"

ComponentRigidBody::ComponentRigidBody() : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = glm::vec3(0);
}

ComponentRigidBody::ComponentRigidBody(float x, float y, float z) : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = glm::vec3(x, y, z);
}

ComponentRigidBody::ComponentRigidBody(glm::vec3 pos) : IComponent(IComponent::ComponentFlags::COMPONENT_POSITION)
{
	position = pos;
}

ComponentRigidBody::~ComponentRigidBody() {}