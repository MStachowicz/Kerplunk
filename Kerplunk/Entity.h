#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>

#include "IComponent.h"
#include "ComponentPosition.h"

class Entity
{
public:
	std::string name; // Name of the entity. Used for debugging purposes.
	std::vector <std::shared_ptr<IComponent>> componentList; // All the components this entity contains
	IComponent::ComponentFlags mask; // The bitwise mask used to match appropriate system actions to this entity.
	static unsigned int entityKey;


	Entity(std::string name);
	~Entity();
	std::shared_ptr<IComponent> FindComponent(int pComponentValue);

	template <typename CompType> 
	void AddComponent(CompType& pComponent) // Adds a single component to the entity and updates the mask.
	{
		componentList.push_back(std::make_shared<CompType>(pComponent));
		updateMask(pComponent);
	}

	// Compares two entities by their adresses to determine if they are the same entity.
	bool operator==(const Entity& rhs) const {
		return
			this == &rhs;
	}

private:
	void updateMask(IComponent &component);  // Updates the mask of the entity with the new component.
};
#endif
