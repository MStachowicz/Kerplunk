#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "IComponent.h"
#include <glm/glm.hpp>
#include <vector>

class ComponentRigidBody : public IComponent
{
public:
	struct Force 
	{
		Force(glm::vec3 pApplicationPoint, glm::vec3 pForce) : applicationPoint (pApplicationPoint), force(pForce)
		{}

		// The point at which the force is being applied to the body
		glm::vec3 applicationPoint;
		// The direction and magnitude of the force
		glm::vec3 force;
	};

	// Current position of the body
	glm::vec3 position;
	// Current velocity of the body
	glm::vec3 velocity;
	// A list of all the forces applied to the body since the last physics tick
	std::vector<Force> forcesApplied;
	// Mass of the body
	float mass;


	ComponentRigidBody();
	ComponentRigidBody(float x, float y, float z);
	ComponentRigidBody(glm::vec3 pos);

	~ComponentRigidBody();

private:
	const float DEFAULTMASS = 1.0f;
	const glm::vec3 DEFAULTVELOCITY = glm::vec3(0.0f, 0.0f, 1.0f);
};
#endif