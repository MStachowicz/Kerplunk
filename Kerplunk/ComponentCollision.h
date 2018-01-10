#ifndef COMPONENTCOLLISION_H
#define COMPONENTCOLLISION_H

#include "IComponent.h"

class ComponentCollision : public IComponent
{
	enum collisionPrimitiveType
	{
		Sphere,
		Cube,
		Point
	};

public:
	collisionPrimitiveType type;

	ComponentCollision(collisionPrimitiveType pType);
	~ComponentCollision();
};
#endif
