#include "SystemManager.h"



SystemManager::SystemManager()
{
}

void SystemManager::AddSystem(ISystem &system)
{
	systemList.push_back(&system);
}


SystemManager::~SystemManager()
{
}
