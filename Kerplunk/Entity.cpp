#include "Entity.h"
#include <iostream>
#include <iterator>

Entity::Entity(std::string pName)
{
	name = pName;
}

std::shared_ptr<IComponent> Entity::FindComponent(int pComponentValue) const
{
	// lambda expression
	auto findComponentType = [pComponentValue](auto component)
	{
		return component->componentType == pComponentValue;
	};

	// return iterator pointer to component being searched for
	auto component_it = std::find_if(componentList.begin(), componentList.end(), findComponentType);

	if (component_it == end(componentList)) // if didnt find element return empty component
	{
		return nullptr;
		//return IComponent(IComponent::COMPONENT_NONE);
	}
	else // otherwise returning dereferenced component in the list of components
	{
		return *component_it;
	}
}


void Entity::updateMask(IComponent &component)
{
	if (component.componentType == IComponent::ComponentFlags::COMPONENT_NONE)
	{
		std::cout << "ERROR ADDING EMPTY COMPONENT TO ENTITY: " << name << std::endl;
	}
	else
	{
		// Update the entity mask with the newly added component type
		unsigned int tempMask = static_cast<int>(mask); // casting mask to int to perform bitwise set operation
		tempMask |= static_cast<int>(component.componentType);
		mask = (IComponent::ComponentFlags)tempMask; // Assign the new value of the mask
	}
}


Entity::~Entity()
{
}
