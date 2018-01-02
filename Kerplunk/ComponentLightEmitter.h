#ifndef COMPONENTLIGHTEMITTER_H
#define COMPONENTLIGHTEMITTER_H

#include "IComponent.h"
#include <glm\glm.hpp>

class ComponentLightEmitter : public IComponent
{
public:
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	ComponentLightEmitter(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular);
	~ComponentLightEmitter();
};
#endif