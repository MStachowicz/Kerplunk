#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ComponentDirectionShadowEmitter : public IComponent
{
public:
	float near;
	float far;
	float size;

	glm::mat4 lightProjection;

	ComponentDirectionShadowEmitter(float pNear, float pFar, float pSize);
	~ComponentDirectionShadowEmitter();
};

