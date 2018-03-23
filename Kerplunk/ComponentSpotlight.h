#ifndef COMPONENTSPOTLIGHT_H
#define COMPONENTSPOTLIGHT_H

#include "IComponent.h"
#include <glm/glm.hpp>

const glm::vec3 DEFAULT_SPOT_POSITION = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_SPOT_DIRECTION = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 DEFAULT_SPOT_COLOUR = glm::vec3(1.0f);
const glm::vec3 DEFAULT_SPOT_SPECULAR = glm::vec3(1.0f);

const float DEFAULT_SPOT_CONSTANT_ATTENUATION = 1.0f;
const float DEFAULT_SPOT_LINEAR_ATTENUATION = 0.09f;
const float DEFAULT_SPOT_QUADRATIC_ATTENUATION = 0.032f;

const float DEFAULT_SPOT_INNER_CUTOFF = glm::cos(glm::radians(12.5f));
const float DEFAULT_SPOT_OUTER_CUTOFF = glm::cos(glm::radians(15.0f));

class ComponentSpotlight : public IComponent
{
public:
	ComponentSpotlight();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float innerCutOff;
	float outerCutOff;


	~ComponentSpotlight();
};
#endif