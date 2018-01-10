#ifndef SYSTEMLIGHTING_H
#define SYSTEMLIGHTING_H

#include <iostream>
#include "ISystem.h"
#include "ComponentLightAttenuation.h"
#include "ComponentLightDirection.h"
#include "ComponentLightCutOff.h"
#include "ComponentLightEmitter.h"
#include "ComponentShader.h"
#include "ComponentCollision.h"
#include "EntityManager.h"

class ComponentRotation;
class ComponentScale;
class EntityManager;

class SystemCollision : public ISystem
{
public:
	SystemCollision(EntityManager& pManager);
	void OnAction(Entity & entity);
	~SystemCollision();
private:
	EntityManager& entityManager;
};
#endif
