#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "IComponent.h"

class Entity
{
public:
	Entity(std::string name);
	void AddComponent(IComponent &component); // Adds a single component to the entity.
	
	std::string name; // Name of the entity. Used for debugging purposes.
	std::vector <IComponent*> componentList; // All the components this entity contains
	IComponent::ComponentFlags mask; // The bitwise mask used to match appropriate system actions to this entity.

	~Entity();
};
#endif
