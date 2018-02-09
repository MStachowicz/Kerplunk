#ifndef SYSTEMLIGHTING_H
#define SYSTEMLIGHTING_H

#include <iostream>
#include "ISystem.h"
#include "ComponentLightAttenuation.h"
#include "ComponentLightDirection.h"
#include "ComponentLightCutOff.h"
#include "ComponentLightEmitter.h"
#include "ComponentShader.h"

class SystemLighting : public ISystem
{
public:
	SystemLighting();
	void Tick(Entity & entity);
	void OnLoad(Entity &entity);
	~SystemLighting();
};
#endif