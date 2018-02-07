#ifndef SYSTEMPHYSICS_H
#define SYSTEMPHYSICS_H

#include "ISystem.h"
#include "IComponent.h"
#include "ComponentPosition.h"
#include "ComponentVelocity.h"
#include "ComponentCollision.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>

class SystemPhysics : public ISystem
{
public:
	SystemPhysics();
	void OnAction(Entity & entity);
	void OnLoad(Entity &entity);
	void Motion(glm::vec3 &pPosition, glm::vec3 &pLastPosition, float deltaTime);
	~SystemPhysics();

private:
	void spherePlaneResponse(Entity &pSphere, Entity &pPlane, ComponentCollision::CollisionData &pCollisionData);
};
#endif