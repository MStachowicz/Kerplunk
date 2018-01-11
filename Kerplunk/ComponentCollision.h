#ifndef COMPONENTCOLLISION_H
#define COMPONENTCOLLISION_H

#include "IComponent.h"
#include "vector"
#include <math.h>

class ComponentCollision : public IComponent
{

public:
	enum collisionPrimitiveType
	{
		Sphere,
		Cube,
		Point
	};

	collisionPrimitiveType type;

	ComponentCollision(collisionPrimitiveType pType);
	~ComponentCollision();
};
#endif
