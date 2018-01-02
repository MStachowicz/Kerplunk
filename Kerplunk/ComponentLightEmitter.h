#ifndef COMPONENTLIGHTEMITTER_H
#define COMPONENTLIGHTEMITTER_H

#include "IComponent.h"
#include <glm\glm.hpp>

class ComponentLightEmitter : public IComponent
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	ComponentLightEmitter(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular);
	~ComponentLightEmitter();
};
#endif