#include "ComponentLightAttenuation.h"

ComponentLightAttenuation::ComponentLightAttenuation(float pConstant, float pLinear, float pQuadratic) : IComponent(IComponent::ComponentFlags::COMPONENT_LIGHT_ATTENUATION),
constant(pConstant), linear(pLinear), quadratic(pQuadratic)
{
}


ComponentLightAttenuation::~ComponentLightAttenuation()
{
}