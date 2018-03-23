// Abstract base class defining a system which will carry out actions on entities.
#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <string>
#include "Entity.h"

class ISystem
{

public:
	ISystem(std::string pName, IComponent::ComponentFlags MASK);

	std::string name;
	const IComponent::ComponentFlags MASK;

	virtual void OnLoad(const std::shared_ptr<Entity> &entity) = 0;
	
	// These methods are called in the following order every frame.

	virtual void OnTickStart(const std::shared_ptr<Entity> &entity) = 0;
	virtual void OnTickStart() = 0;
	virtual void Tick(const std::shared_ptr<Entity> &entity) = 0;
	virtual ~ISystem();
};
#endif