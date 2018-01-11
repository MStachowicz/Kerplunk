#ifndef SYSTEMCOLLISION_H
#define SYSTEMCOLLISION_H

#include <iostream>
#include <glm\glm.hpp>
#include "ISystem.h"
#include "ComponentCollision.h"
#include "EntityManager.h"
#include "ComponentVelocity.h"
#include "ComponentScale.h"
#include "EntityManager.h"

class EntityManager;

class SystemCollision : public ISystem
{
public:
	SystemCollision(EntityManager& pManager);
	void OnAction(Entity & entity);
	~SystemCollision();
private:
	EntityManager& entityManager;
};
#endif
