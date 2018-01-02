#ifndef SYSTEMLIGHTING_H
#define SYSTEMLIGHTING_H

#include "ISystem.h"

class SystemLighting : public ISystem
{
public:
	SystemLighting();
	void OnAction(Entity & entity);
	~SystemLighting();
};
#endif