#include "SystemLighting.h"

SystemLighting::SystemLighting() : ISystem("SystemLighting", (IComponent::ComponentFlags)(IComponent::COMPONENT_LIGHT_EMITTER | IComponent::COMPONENT_SHADER))
{
}

void SystemLighting::OnLoad(Entity & entity) 
{
	if ((entity.mask & MASK) == MASK)
	{
	}
}

void SystemLighting::Tick(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		// If the light has direction must be spotlight or directional light
		if (((entity.mask & IComponent::COMPONENT_LIGHT_DIRECTION) == IComponent::COMPONENT_LIGHT_DIRECTION))
		{
			// If the light has a cutoff it must be a spotlight
			if (((entity.mask & IComponent::COMPONENT_LIGHT_CUT_OFF) == IComponent::COMPONENT_LIGHT_CUT_OFF))
			{
				// Check if spotlight contains the remaining required components (position and attenuation) before fetching them.
				if (((entity.mask & IComponent::COMPONENT_POSITION) == IComponent::COMPONENT_POSITION) &&
					((entity.mask & IComponent::COMPONENT_LIGHT_ATTENUATION) == IComponent::COMPONENT_LIGHT_ATTENUATION))
				{
					// SPOTLIGHT 
					// ---------------------------------------------------------------------------------------------------------------------
					std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
					std::shared_ptr<ComponentLightDirection> directionComp = std::dynamic_pointer_cast<ComponentLightDirection> (entity.FindComponent(4096));
					std::shared_ptr<ComponentLightEmitter> emitterComp = std::dynamic_pointer_cast<ComponentLightEmitter> (entity.FindComponent(1024));
					std::shared_ptr<ComponentLightAttenuation> attenuationComp = std::dynamic_pointer_cast<ComponentLightAttenuation> (entity.FindComponent(2048));
					std::shared_ptr<ComponentLightCutOff> cutoffComp = std::dynamic_pointer_cast<ComponentLightCutOff> (entity.FindComponent(8192));
					std::shared_ptr<ComponentShader> shaderComp = std::dynamic_pointer_cast<ComponentShader> (entity.FindComponent(128));
					
					// Setting the uniforms for the spotlight
					shaderComp->shader->setVec3("spotlight.position", posComp->position);
					shaderComp->shader->setVec3("spotlight.direction", directionComp->direction);
					shaderComp->shader->setVec3("spotlight.ambient", emitterComp->ambient);
					shaderComp->shader->setVec3("spotlight.diffuse", emitterComp->diffuse);
					shaderComp->shader->setVec3("spotlight.specular", emitterComp->specular);
					shaderComp->shader->setFloat("spotlight.constant", attenuationComp->constant);
					shaderComp->shader->setFloat("spotlight.linear", attenuationComp->linear);
					shaderComp->shader->setFloat("spotlight.quadratic", attenuationComp->quadratic);
					shaderComp->shader->setFloat("spotlight.cutOff", glm::cos(glm::radians(cutoffComp->innerCutOff)));
					shaderComp->shader->setFloat("spotlight.outerCutOff", glm::cos(glm::radians(cutoffComp->outerCutOff)));
				}
				else
				{
					std::cout << "ERROR::SYSTEMLIGHTING light emitter (spotlight) with no position or attenuation" << std::endl;
				}
			}
			else // light with direction and no cut off must be a directional light
			{
				// DIRECTIONAL LIGHT 
				// ---------------------------------------------------------------------------------------------------------------------
				std::shared_ptr<ComponentLightDirection> directionComp = std::dynamic_pointer_cast<ComponentLightDirection> (entity.FindComponent(4096));
				std::shared_ptr<ComponentLightEmitter> emitterComp = std::dynamic_pointer_cast<ComponentLightEmitter> (entity.FindComponent(1024));
				std::shared_ptr<ComponentShader> shaderComp = std::dynamic_pointer_cast<ComponentShader> (entity.FindComponent(128));

				// Setting the uniforms for the spotlight
				shaderComp->shader->setVec3("dirLight.direction", directionComp->direction);
				shaderComp->shader->setVec3("dirLight.ambient", emitterComp->ambient);
				shaderComp->shader->setVec3("dirLight.diffuse", emitterComp->diffuse);
				shaderComp->shader->setVec3("dirLight.specular", emitterComp->specular);
			}
		}
		else // light with no direction must be a point light
		{
			// POINT LIGHT 
			// ---------------------------------------------------------------------------------------------------------------------
			std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
			std::shared_ptr<ComponentLightEmitter> emitterComp = std::dynamic_pointer_cast<ComponentLightEmitter> (entity.FindComponent(1024));
			std::shared_ptr<ComponentLightAttenuation> attenuationComp = std::dynamic_pointer_cast<ComponentLightAttenuation> (entity.FindComponent(2048));
			std::shared_ptr<ComponentShader> shaderComp = std::dynamic_pointer_cast<ComponentShader> (entity.FindComponent(128));

			int i = 0;
			std::string number = std::to_string(i);
			
			shaderComp->shader->use();

			// Setting the uniforms for the spotlight
			shaderComp->shader->setVec3("pointLights[" + number + "].position", posComp->position);
			shaderComp->shader->setVec3("pointLights[" + number + "].ambient", emitterComp->ambient);
			shaderComp->shader->setVec3("pointLights[" + number + "].diffuse", emitterComp->diffuse);
			shaderComp->shader->setVec3("pointLights[" + number + "].specular", emitterComp->specular);
			shaderComp->shader->setFloat("pointLights[" + number + "].constant", attenuationComp->constant);
			shaderComp->shader->setFloat("pointLights[" + number + "].linear", attenuationComp->linear);
			shaderComp->shader->setFloat("pointLights[" + number + "].quadratic", attenuationComp->quadratic);
		}
	}
}


SystemLighting::~SystemLighting()
{
}
