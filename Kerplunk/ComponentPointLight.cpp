#include "ComponentPointLight.h"



ComponentPointLight::ComponentPointLight() : IComponent(IComponent::ComponentFlags::COMPONENT_POINTLIGHT),
position(DEFAULT_POINT_POSITION), 
ambient(DEFAULT_POINT_COLOUR), diffuse(DEFAULT_POINT_COLOUR), 
specular(DEFAULT_POINT_SPECULAR),
constant(DEFAULT_POINT_CONSTANT_ATTENUATION),
linear(DEFAULT_POINT_LINEAR_ATTENUATION),
quadratic(DEFAULT_POINT_QUADRATIC_ATTENUATION)
{}


ComponentPointLight::~ComponentPointLight()
{
}
