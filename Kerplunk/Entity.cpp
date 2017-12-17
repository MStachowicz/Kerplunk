#include "Entity.h"

#include <iostream>

Entity::Entity(std::string pName)
{
	name = pName;
}

void Entity::AddComponent(IComponent &component)
{
	if (component.componentType == IComponent::ComponentFlags::COMPONENT_NONE)
	{
		std::cout << "ERROR ADDING EMPTY COMPONENT TO ENTITY: " << name << std::endl;
	}
	else
	{
		componentList.push_back(&component);

		// Update the entity mask with the newly added component type
		unsigned int tempMask = static_cast<int>(mask); // casting mask to int to perform bitwise set operation
		tempMask |= static_cast<int>(component.componentType);
		mask = (IComponent::ComponentFlags)tempMask; // Assign the new value of the mask
	}
}

Entity::~Entity()
{
}
