#ifndef SYSTEMCOLLISION_H
#define SYSTEMCOLLISION_H

#include <iostream>
#include <glm\glm.hpp>
#include "ISystem.h"
#include "EntityManager.h"
#include "ComponentCollision.h"
#include "ComponentVelocity.h"
#include "ComponentScale.h"

class EntityManager;

class SystemCollision : public ISystem
{
public:
	SystemCollision(EntityManager& pManager);
	void Tick(Entity &entity);
	void OnLoad(Entity &entity);
	~SystemCollision();
private:
	EntityManager& entityManager;
	bool CollisionSpherePlane(Entity & pSphereEntity, Entity &pCubeEntity);
	glm::vec3 ClosestPtPlane(Entity& pPlane, glm::vec3& pPoint);
};
#endif
