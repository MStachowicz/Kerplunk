#include "ComponentLightCutOff.h"

ComponentLightCutOff::ComponentLightCutOff(float pInnerCutOff, float pOuterCutOff) : IComponent(IComponent::COMPONENT_LIGHT_CUT_OFF),
innerCutOff(pInnerCutOff), outerCutOff(pOuterCutOff)
{
}


ComponentLightCutOff::~ComponentLightCutOff()
{
}
