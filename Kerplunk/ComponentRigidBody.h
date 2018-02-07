#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "IComponent.h"
#include <glm/glm.hpp>

class ComponentRigidBody : public IComponent
{
public:
	glm::vec3 position;
	glm::vec3 velocity;


	ComponentRigidBody();
	ComponentRigidBody(float x, float y, float z);
	ComponentRigidBody(glm::vec3 pos);

	~ComponentRigidBody();
};
#endif