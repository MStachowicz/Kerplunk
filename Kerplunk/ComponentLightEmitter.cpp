#include "ComponentLightEmitter.h"

ComponentLightEmitter::ComponentLightEmitter(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular) : IComponent(ComponentFlags::COMPONENT_LIGHT_EMITTER),
ambient(pAmbient), diffuse(pDiffuse), specular(pSpecular)
{
}


ComponentLightEmitter::~ComponentLightEmitter()
{
}
