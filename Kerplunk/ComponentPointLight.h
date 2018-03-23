#ifndef COMPONENTPOINTLIGHT_H
#define COMPONENTPOINTLIGHT_H

#include "IComponent.h"
#include <glm/glm.hpp>

const glm::vec3 DEFAULT_POINT_POSITION = glm::vec3(8.0f, 10.0f, 0.0f);
const glm::vec3 DEFAULT_POINT_COLOUR = glm::vec3(1.0f);
const glm::vec3 DEFAULT_POINT_SPECULAR = glm::vec3(1.0f);

const float DEFAULT_POINT_CONSTANT_ATTENUATION = 1.0f;
const float DEFAULT_POINT_LINEAR_ATTENUATION = 0.9f;
const float DEFAULT_POINT_QUADRATIC_ATTENUATION = 0.32f;

class ComponentPointLight : public IComponent
{
public:
	ComponentPointLight();

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	~ComponentPointLight();
};
#endif