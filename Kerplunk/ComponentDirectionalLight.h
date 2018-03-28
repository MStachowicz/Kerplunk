#ifndef COMPONENTDIRECTIONALIGHT_H
#define COMPONENTDIRECTIONALIGHT_H

#include "IComponent.h"
#include <glm/glm.hpp>

const glm::vec3 DEFAULT_DIR_POSITION = glm::vec3(0.0f, 30.0f, 0.0f);
const glm::vec3 DEFAULT_DIR_DIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 DEFAULT_DIR_AMBIENT = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_DIR_DIFFUSE = glm::vec3(0.7f, 0.7f, 0.7f);
const glm::vec3 DEFAULT_DIR_SPECULAR = glm::vec3(0.1f, 0.1f, 0.1f);

class ComponentDirectionalLight : public IComponent
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	ComponentDirectionalLight();
	~ComponentDirectionalLight();
};
#endif
