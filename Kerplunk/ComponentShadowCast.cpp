#include "ComponentShadowCast.h"

ComponentShadowCast::ComponentShadowCast() : IComponent(IComponent::ComponentFlags::COMPONENT_SHADOW_CASTER)
{
	const unsigned int SHADOW_WIDTH = 1024 * 2, SHADOW_HEIGHT = 1024 * 2;

	// generating the FBO and 2D texture to be used as the FBO's depth buffer
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &directionalShadowDepthMap);

	glBindTexture(GL_TEXTURE_2D, directionalShadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // setting border to prevent shadow in samples outside the light perspective
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attaching the depth texture to the framebuffers depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, directionalShadowDepthMap, 0);

	// No need to attach a colour buffer so setting draw and read buffer to GL_NONE to signify this.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentShadowCast::setLightSpaceMatrix(const glm::vec3 &pPosition)
{
	if (auto shader = depthShader.lock()) {

		glm::mat4 lightProjection, lightView;
		float near_plane = 1.0f, far_plane = 50.0f, projSize = 50.0f; // todo: make these variable members

		lightProjection = glm::ortho(-projSize, projSize, -projSize, projSize, near_plane, far_plane);
		lightView = glm::lookAt(
			glm::vec3(pPosition), // light position
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		shader->use(); // todo: test if use needs to be called to set variable
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	}
	else
	{
		std::cout << "ERROR SHADOW CASTER: NO DEPTH SHADER ATTACHED" << std::endl;
	}
}

ComponentShadowCast::~ComponentShadowCast()
{}