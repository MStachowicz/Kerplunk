#include "ComponentDirectionalLight.h"

ComponentDirectionalLight::ComponentDirectionalLight() : IComponent(IComponent::ComponentFlags::COMPONENT_DIRECTIONAL_LIGHT),
direction(DEFAULT_DIR_DIRECTION),
ambient(DEFAULT_DIR_COLOUR),
diffuse(DEFAULT_DIR_COLOUR),
specular(DEFAULT_DIR_SPECULAR)
{}

ComponentDirectionalLight::~ComponentDirectionalLight()
{}