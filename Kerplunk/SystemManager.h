#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "ISystem.h"
#include <vector>

class SystemManager
{
public:
	std::vector<ISystem*> systemList;

	SystemManager();

	void AddSystem(ISystem &system);
	//void ActionSystems(EntityManager &manager)

	~SystemManager();
};

#endif

