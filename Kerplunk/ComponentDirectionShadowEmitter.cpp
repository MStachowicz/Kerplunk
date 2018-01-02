#include "ComponentDirectionShadowEmitter.h"

ComponentDirectionShadowEmitter::ComponentDirectionShadowEmitter(float pNear, float pFar, float pSize) : IComponent(IComponent::COMPONENT_DIRECTION_SHADOW_EMITTER),
near(pNear), far(pFar), size(pSize)
{
	lightProjection = glm::ortho(-size, size, -size, size, near, far);
}


ComponentDirectionShadowEmitter::~ComponentDirectionShadowEmitter()
{
}