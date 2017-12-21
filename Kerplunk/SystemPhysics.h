#ifndef SYSTEMPHYSICS_H
#define SYSTEMPHYSICS_H

#include "ISystem.h"
#include "IComponent.h"
#include "ComponentPosition.h"
#include "ComponentVelocity.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm\glm.hpp>

class SystemPhysics : public ISystem
{
public:
	SystemPhysics();
	void OnAction(Entity & entity);
	void Motion(glm::vec3 &pPosition, glm::vec3 pVelocity,  float deltaTime);
	~SystemPhysics();
};
#endif