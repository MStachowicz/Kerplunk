#include "EntityManager.h"

EntityManager::EntityManager() {}


void EntityManager::AddEntity(Entity entity)
{
	entityList.push_back(entity);
}

void EntityManager::FindEntity(std::string name)
{
}

EntityManager::~EntityManager() {}
