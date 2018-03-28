#include "ComponentDirectionalLight.h"

ComponentDirectionalLight::ComponentDirectionalLight() : IComponent(IComponent::ComponentFlags::COMPONENT_DIRECTIONAL_LIGHT),
position(DEFAULT_DIR_POSITION),
direction(DEFAULT_DIR_DIRECTION),
ambient(DEFAULT_DIR_AMBIENT),
diffuse(DEFAULT_DIR_DIFFUSE),
specular(DEFAULT_DIR_SPECULAR)
{}

ComponentDirectionalLight::~ComponentDirectionalLight()
{}