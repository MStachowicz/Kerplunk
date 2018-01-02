#include "SystemLighting.h"

SystemLighting::SystemLighting() : ISystem("SystemLighting", (IComponent::ComponentFlags)(IComponent::COMPONENT_LIGHT_EMITTER | IComponent::COMPONENT_SHADER))
{
}

void SystemLighting::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		// If the light has direction must be spotlight or directional light
		if (((entity.mask & IComponent::COMPONENT_LIGHT_DIRECTION) == IComponent::COMPONENT_LIGHT_DIRECTION))		
		{
			// If the light has a cutoff it must be a spotlight
			if (((entity.mask & IComponent::COMPONENT_LIGHT_CUTOFF) == IComponent::COMPONENT_LIGHT_CUTOFF))
			{

			}
			else // light with direction and no cut off must be a directional light
			{

			}
		}
		else // light with no direction must be a point light
		{

		}
	}
}

SystemLighting::~SystemLighting()
{
}
