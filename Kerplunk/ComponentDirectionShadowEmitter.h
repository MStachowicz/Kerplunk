#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class ComponentDirectionShadowEmitter : public IComponent
{
public:
	float near;
	float far;
	float size;

	unsigned int FBO_Handle;
	unsigned int depthTexture;
	const unsigned int RESOLUTIONX = 1024;
	const unsigned int RESOLUTIONY = 1024;

	glm::mat4 lightProjection;

	ComponentDirectionShadowEmitter(float pNear, float pFar, float pSize);
	~ComponentDirectionShadowEmitter();

private:
	void setupFBO(); // Generates the FBO and texture depth attachment of the FBO.
};

