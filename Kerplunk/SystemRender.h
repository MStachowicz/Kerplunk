#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "ISystem.h"
#include "IComponent.h"
#include "ComponentPosition.h"
#include "ComponentGeometry.h"
#include <glm\glm.hpp>

class SystemRender : public ISystem
{
public:
	SystemRender();
	void OnAction(Entity &entity);
	~SystemRender();
};
#endif