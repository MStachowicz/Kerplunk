#ifndef SYSTEMPHYSICS_H
#define SYSTEMPHYSICS_H

#include "ISystem.h"
#include "IComponent.h"
#include "ComponentPosition.h"
#include "ComponentVelocity.h"
#include "ComponentCollision.h"
#include "ComponentRigidBody.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>

//class ComponentRigidBody;

class SystemPhysics : public ISystem
{
public:
	// The current timeStep being applied to physics calculations
	float timeStep = 0;


	SystemPhysics();
	void OnAction(Entity & entity);
	void OnLoad(Entity &entity);
	void RigidBodyMotion(glm::vec3 &pPosition, glm::vec3 &pVelocity, float deltaTime);
	~SystemPhysics();

private:
	void spherePlaneResponse(Entity &pSphere, Entity &pPlane, ComponentCollision::CollisionData &pCollisionData);
};
#endif