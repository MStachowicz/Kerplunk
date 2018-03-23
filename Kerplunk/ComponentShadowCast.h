#ifndef COMPONENTSHADOWCAST_H
#define COMPONENTSHADOWCAST_H

#include "IComponent.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class ComponentShadowCast : public IComponent
{
public:
	ComponentShadowCast();

	// Shader used to fill the depth buffer. Assigned by the lighting system. Filled in the rendering system.
	std::weak_ptr<Shader> depthShader;
	// Frame buffer object storing the depth texture.
	unsigned int depthMapFBO;
	// 2D texture attached to the FBO as a depth buffer.
	unsigned int directionalShadowDepthMap;
	// Matrix used in the directional light shadow calculations in the lighting shader. Used to transform
	// fragment positions to light space.
	glm::mat4 lightSpaceMatrix;
	// Values used to set the viewport when rendering all the entities which emit shadows.
	const unsigned int SHADOW_WIDTH = 1024 * 2, SHADOW_HEIGHT = 1024 * 2;

	void setLightSpaceMatrix(const glm::vec3 &pPosition);

	~ComponentShadowCast();
};
#endif