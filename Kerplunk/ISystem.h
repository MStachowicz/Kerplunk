// Abstract base class defining a system which will carry out actions on entities.
#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <string>

class ISystem
{

public:

	std::string name;

	virtual void OnAction() = 0;

	~ISystem();
};
#endif