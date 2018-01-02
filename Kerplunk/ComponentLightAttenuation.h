#ifndef COMPONENTLIGHTATTENUATION_H
#define COMPONENTLIGHTATTENUATION_H

#include "IComponent.h"

class ComponentLightAttenuation : public IComponent
{
public:
	float constant;
	float linear;
	float quadratic;

	ComponentLightAttenuation(float pConstant, float pLinear, float pQuadratic);
	~ComponentLightAttenuation();
};
#endif