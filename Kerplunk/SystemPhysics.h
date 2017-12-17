#ifndef SYSTEMPHYSICS_H
#define SYSTEMPHYSICS_H

#include "ISystem.h"
#include "IComponent.h"

class SystemPhysics : public ISystem
{
public:
	SystemPhysics();
	void OnAction(Entity & entity);
	~SystemPhysics();
};
#endif