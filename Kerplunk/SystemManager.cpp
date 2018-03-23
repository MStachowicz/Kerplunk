#include "SystemManager.h"

SystemManager::SystemManager()
{
}

// Adds a system to the system list.
void SystemManager::AddSystem(ISystem &system)
{
	systemList.push_back(&system);
}
// Called every frame. Calls every system OnFrameStart method and then every system Tick method for every
// entity in the manager.
void SystemManager::ActionSystems(EntityManager & manager)
{
	for (int i = 0; i < systemList.size(); i++)
	{
		for (int j = 0; j < manager.entityList.size(); j++)
		{
			systemList[i]->OnTickStart(manager.entityList[j]);
		}
	}

	for (int i = 0; i < systemList.size(); i++)
	{
		systemList[i]->OnTickStart();

		for (int j = 0; j < manager.entityList.size(); j++)
		{
			systemList[i]->Tick(manager.entityList[j]);
		}
	}
}

void SystemManager::LoadSystems(EntityManager &manager)
{
	for (int i = 0; i < systemList.size(); i++)
	{
		for (int j = 0; j < manager.entityList.size(); j++)
		{
			systemList[i]->OnLoad(manager.entityList[j]);
		}
	}
}


SystemManager::~SystemManager() {}
