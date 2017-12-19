#ifndef SYSTEMPHYSICS_H
#define SYSTEMPHYSICS_H

#include "ISystem.h"
#include "IComponent.h"
#include "ComponentPosition.h"
#include "ComponentVelocity.h"
#include <glm\glm.hpp>

class SystemPhysics : public ISystem
{
public:
	SystemPhysics();
	void OnAction(Entity & entity);
	void Motion(glm::vec3 &pPosition, float deltaTime);
	~SystemPhysics();
};
#endif