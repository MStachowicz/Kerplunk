#ifndef SYSTEMPHYSICS.H
#define SYSTEMPHYSICS.H

#include "ISystem.h"

class SystemPhysics : public ISystem
{
public:
	SystemPhysics();

	void ISystem::OnAction(Entity &entity);

	~SystemPhysics();
};
#endif