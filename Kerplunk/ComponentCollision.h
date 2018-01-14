#ifndef COMPONENTCOLLISION_H
#define COMPONENTCOLLISION_H

#include <math.h>
#include "vector"

#include <iostream>
#include <glm\glm.hpp>
#include "ISystem.h"
#include "IComponent.h"
#include "ComponentCollision.h"
#include "ComponentVelocity.h"
#include "ComponentScale.h"

class Entity;

class ComponentCollision : public IComponent
{

public:
	enum collisionPrimitiveType
	{
		Sphere,
		Plane,
		Cube,
		Point
	};

	// Stores all the information required to perform a collision response.
	struct CollisionData
	{
		CollisionData(Entity& pEntityCollidedWith, glm::vec3 pCollisionPoint) : entityCollidedWith(pEntityCollidedWith), collisionPoint(pCollisionPoint)
		{ }

		// The entity this entity collided with.
		Entity& entityCollidedWith;
		// The point of the collision.
		glm::vec3 collisionPoint;
	};

	collisionPrimitiveType type;
	std::vector<CollisionData> collisions;

	void AddCollision(Entity &entityCollidedWith, glm::vec3 &collisionPoint);

	ComponentCollision(collisionPrimitiveType pType);
	~ComponentCollision();
};
#endif
