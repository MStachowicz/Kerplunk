#ifndef COMPONENTLIGHTCUTOFF_H
#define COMPONENTLIGHTCUTOFF_H

#include "IComponent.h"

class ComponentLightCutOff : public IComponent
{
public:
	float innerCutOff;
	float outerCutOff;

	ComponentLightCutOff(float pInnerCutOff, float pOuterCutOff);
	~ComponentLightCutOff();
};
#endif