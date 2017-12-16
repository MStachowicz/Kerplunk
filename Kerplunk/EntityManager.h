#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include<string>
#include "Entity.h"

class EntityManager
{
public:
	EntityManager();

	std::vector<Entity*> entityList;

	// Adds an entity to the entity list of the manager.
	void AddEntity(Entity &entity);
	// Finds an entity in the entity list by its name.
	void FindEntity(std::string name);

	~EntityManager();
};
#endif