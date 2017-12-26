#ifndef COMPONENTMATERIAL_H
#define COMPONENTMATERIAL_H

#include "IComponent.h"
#include <glm/glm.hpp>

// Allows an entity to be rendered with a material surface as opposed to a textured one.
class ComponentMaterial : public IComponent
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	ComponentMaterial();
	ComponentMaterial(glm::vec3 pAmbientDiffuse);
	ComponentMaterial(glm::vec3 pAmbientDiffuse, glm::vec3 pSpecular);
	ComponentMaterial(glm::vec3 pAmbientDiffuse, glm::vec3 pSpecular, float pShininess);
	ComponentMaterial(glm::vec3 pAmbient, glm::vec3 pDiffuse, glm::vec3 pSpecular, float pShininess);

	~ComponentMaterial();
};
#endif