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
	// The total time the simulation has been running
	float SimulationTime = 0;
	// Time since the last physics tick
	float tickDeltaTime;
	// Time of the last physics tick
	float tickLastTime;
	// Whether the gravity is being applied to all the rigid bodies
	bool applyGravity = true;
	// Whether the simulation is paused.
	bool pauseSimulation = false;

	SystemPhysics();
	void Tick(Entity & entity);
	void OnLoad(Entity &entity);
	void UpdateBodyPosition(glm::vec3 &pPosition, glm::vec3 &pVelocity, float tickDeltaTime);
	void UpdateBodyVelocity(glm::vec3 &pVelocity, glm::vec3 &resultantForce, const float &pMass, const float &tickDeltaTime);
	void UpdateTiming(float &tickDeltaTime);
	~SystemPhysics();

private:
	void spherePlaneResponse(Entity &pSphere, Entity &pPlane, ComponentCollision::CollisionData &pCollisionData);
};
#endif