#include "SystemRender.h"

SystemRender::SystemRender() : ISystem("SystemRender", (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY | IComponent::COMPONENT_RENDERABLE))
{}
SystemRender::~SystemRender() {}

void SystemRender::OnLoad(const std::shared_ptr<Entity> &entity)
{
	if ((entity->mask & MASK) == MASK)
	{
		
	}
}
void SystemRender::OnTickStart(const std::shared_ptr<Entity>& entity)
{
}
void SystemRender::OnTickStart()
{
	// reset viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void SystemRender::Tick(const std::shared_ptr<Entity> &entity)
{
	// Rendering Rigid bodies
	if ((entity->mask & MASK) == MASK)
	{
		// Entity must also contain a component to define its geometry either through a model file or a geometric definition.
		if (((entity->mask & IComponent::COMPONENT_GEOMETRY) == IComponent::COMPONENT_GEOMETRY) ||
			((entity->mask & IComponent::COMPONENT_MODEL) == IComponent::COMPONENT_MODEL))
		{
			// Entity must also contain a component to define the way it will be shaded.
			if (((entity->mask & IComponent::COMPONENT_TEXTURE) == IComponent::COMPONENT_TEXTURE) ||
				((entity->mask & IComponent::COMPONENT_MATERIAL) == IComponent::COMPONENT_MATERIAL))
			{

				std::shared_ptr<ComponentRenderable> renderableComp = std::dynamic_pointer_cast<ComponentRenderable> (entity->FindComponent(2));

				if (auto shader = renderableComp->lightingShader.lock()) {
					shader->use();

					std::shared_ptr<ComponentRigidBody> RigidBodyComponent = std::dynamic_pointer_cast<ComponentRigidBody> (entity->FindComponent(65536));


					// Setting the model matrix of the entity
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, RigidBodyComponent->position);

					// Apply rotation
					model = glm::rotate(model, glm::radians(RigidBodyComponent->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(RigidBodyComponent->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::rotate(model, glm::radians(RigidBodyComponent->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					model = glm::scale(model, glm::vec3(RigidBodyComponent->scale));

					shader->setMat4("model", model);


					shader->setBool("isNormalMap", false); // normal mapping not implemented for component architecture

					if ((entity->mask & IComponent::COMPONENT_TEXTURE) == IComponent::COMPONENT_TEXTURE) // textures entities
					{
						std::shared_ptr<ComponentTexture> texComp =
							std::dynamic_pointer_cast<ComponentTexture> (entity->FindComponent(256));

						// Binding textures on shader
						shader->setBool("isTextured", true);

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texComp->diffuseTexture->textureID);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, texComp->specularTexture->textureID);
					}
					else // material entities
					{
						std::shared_ptr<ComponentMaterial> materialComp =
							std::dynamic_pointer_cast<ComponentMaterial> (entity->FindComponent(512));

						// Setting shader material properties
						shader->setBool("isTextured", false);
						// Set uniforms for the material properties
						shader->setVec3("material.ambient", materialComp->ambient);
						shader->setVec3("material.diffuse", materialComp->diffuse);
						shader->setVec3("material.specular", materialComp->specular);
						shader->setFloat("material.shininess", materialComp->shininess);
					}

					// DRAW
					// entity can be drawn using geometry or a model loaded in using assimp
					if ((entity->mask & IComponent::COMPONENT_GEOMETRY) == IComponent::COMPONENT_GEOMETRY) // geometry entities
					{
						std::shared_ptr<ComponentGeometry> geomComp = std::dynamic_pointer_cast<ComponentGeometry> (entity->FindComponent(64));

						// Render the entity
						//glDisable(GL_CULL_FACE);
						geomComp->geometry->render();
						//glEnable(GL_CULL_FACE);
					}
					else // model draw
					{
						std::shared_ptr<ComponentModel> modelComp = std::dynamic_pointer_cast<ComponentModel> (entity->FindComponent(32));

						if (modelComp->useModelTextures)
							modelComp->model->Draw(*shader); // Binds the textures associated with the model
						else
							modelComp->model->Draw(); // Draws the model using the textures or material components
					}
				}
				else
				{
					std::cout << "ERROR SYSTEM RENDER: entity \"" << entity->name << "\" does not have shader attached to its shader component for lighting." << std::endl;
				}
			}
		}
	}
}