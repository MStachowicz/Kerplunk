#include "EntityManager.h"

EntityManager::EntityManager() {}


void EntityManager::AddEntity(Entity entity)
{
	entityList.push_back(std::make_shared<Entity>(entity));
}

void EntityManager::FindEntity(std::string name)
{
}

EntityManager::~EntityManager() {}
