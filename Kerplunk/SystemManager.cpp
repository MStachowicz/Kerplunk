#include "SystemManager.h"

SystemManager::SystemManager()
{
}

// Adds a system to the system list.
void SystemManager::AddSystem(ISystem &system)
{
	systemList.push_back(&system);
}
// Calls every system action on every entity. Performs the actions if the mask of entity and system match.
void SystemManager::ActionSystems(EntityManager & manager)
{
	for (int i = 0; i < systemList.size(); i++) 
	{
		for (int j = 0; j < manager.entityList.size(); j++) 
		{
			systemList[i]->OnAction(manager.entityList[j]);
		}
	}
}


SystemManager::~SystemManager() {}
