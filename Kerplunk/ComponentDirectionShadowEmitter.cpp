#include "ComponentDirectionShadowEmitter.h"

ComponentDirectionShadowEmitter::ComponentDirectionShadowEmitter(float pNear, float pFar, float pSize) : IComponent(IComponent::COMPONENT_DIRECTION_SHADOW_EMITTER),
near(pNear), far(pFar), size(pSize)
{
	lightProjection = glm::ortho(-size, size, -size, size, near, far);
	setupFBO();
}


void ComponentDirectionShadowEmitter::setupFBO()
{
	// Generating the framebuffer object
	glGenFramebuffers(1, &FBO_Handle);

	// Creating a 2D texture to be used as the frame buffers depth buffer
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, RESOLUTIONX, RESOLUTIONY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // setting border to prevent shadow in samples outside the light perspective
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attaching the depth texture to the framebuffers depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_Handle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	// No need to attach a colour buffer so setting draw and read buffer to GL_NONE to signify this.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ComponentDirectionShadowEmitter::~ComponentDirectionShadowEmitter()
{
}