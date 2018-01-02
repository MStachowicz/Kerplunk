#include "ComponentLightEmitter.h"

ComponentLightEmitter::ComponentLightEmitter(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular) : IComponent(ComponentFlags::COMPONENT_LIGHT_EMITTER),
Ambient(pAmbient), Diffuse(pDiffuse), Specular(pSpecular)
{
}


ComponentLightEmitter::~ComponentLightEmitter()
{
}
