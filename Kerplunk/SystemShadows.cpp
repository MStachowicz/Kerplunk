#include "SystemShadows.h"

SystemShadows::SystemShadows(EntityManager& pManager) : ISystem("SystemShadows", (IComponent::ComponentFlags)(IComponent::COMPONENT_SHADOW_CASTER)),
directionalDepthShader(std::make_shared<Shader>("../Kerplunk/depthShader.vert", "../Kerplunk/depthShader.frag", nullptr))
{}
SystemShadows::~SystemShadows()
{}

void SystemShadows::OnLoad(const std::shared_ptr<Entity>& entity)
{
	if ((entity->mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentShadowCast> shadowCastComp = std::dynamic_pointer_cast<ComponentShadowCast> (entity->FindComponent(128));

		shadowCastComp->depthShader = directionalDepthShader;
	}

	// Add all the entities which can be rendered to the member vector
	IComponent::ComponentFlags drawableMask = (IComponent::ComponentFlags)(IComponent::COMPONENT_GEOMETRY | IComponent::COMPONENT_MODEL);
	IComponent::ComponentFlags surfaceMask = (IComponent::ComponentFlags)(IComponent::COMPONENT_TEXTURE | IComponent::COMPONENT_MATERIAL);
	IComponent::ComponentFlags RenderSystemMask = (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY | IComponent::COMPONENT_RENDERABLE);

	// If the entity is renederable
	if ((entity->mask & RenderSystemMask) == RenderSystemMask)
	{
		if ((entity->mask & drawableMask) != 0)
		{
			if ((entity->mask & surfaceMask) != 0)
			{
				renderableEntities.push_back(entity);
			}
		}
	}

}

void SystemShadows::OnTickStart(const std::shared_ptr<Entity>& entity)
{
	if ((entity->mask & MASK) == MASK)
	{
		FillDepthBuffer(entity);
	}
}
void SystemShadows::OnTickStart()
{
}
void SystemShadows::Tick(const std::shared_ptr<Entity>& entity)
{
	if ((entity->mask & MASK) == MASK)
	{
	}
}

void SystemShadows::FillDepthBuffer(const std::shared_ptr<Entity> &entity)
{
	std::shared_ptr<ComponentShadowCast> shadowCastComp = std::dynamic_pointer_cast<ComponentShadowCast> (entity->FindComponent(128));

	if (auto shader = shadowCastComp->depthShader.lock()) {

		// hard coded to work with directional light as consider moving all shadow functionality to new system avoiding low cohesion
		std::shared_ptr<ComponentDirectionalLight> directionLightComp = std::dynamic_pointer_cast<ComponentDirectionalLight> (entity->FindComponent(4));

		shadowCastComp->setLightSpaceMatrix(glm::vec3(-2.0f, 30.0f, -1.0f));

		glViewport(0, 0, shadowCastComp->SHADOW_WIDTH, shadowCastComp->SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowCastComp->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT); // reverse cull order to reduce peter panning on shadows

		for (size_t i = 0; i < renderableEntities.size(); i++) {
			if (auto entity = renderableEntities[i].lock()) {

				std::shared_ptr<ComponentRenderable> renderableComp = std::dynamic_pointer_cast<ComponentRenderable> (entity->FindComponent(2));
				
				if (renderableComp->emitShadows)
				{
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

					// DRAW
					if ((entity->mask & IComponent::COMPONENT_GEOMETRY) == IComponent::COMPONENT_GEOMETRY) // geometry draw
					{
						std::shared_ptr<ComponentGeometry> geomComp = std::dynamic_pointer_cast<ComponentGeometry> (entity->FindComponent(64));
						geomComp->geometry->render();
					}
					else // model draw
					{
						std::shared_ptr<ComponentModel> modelComp = std::dynamic_pointer_cast<ComponentModel> (entity->FindComponent(32));
						modelComp->model->Draw();
					}
				}
			}
			else
				std::cout << "ERROR SYSTEM RENDER: renderable entities vector contains weak pointer not intialized at index: " << i << std::endl;
		}

		// reset the cull order and unbind the framebuffer
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
		std::cout << "ERROR SYSTEM RENDER: entity \"" << entity->name << "\" does not have depth shader attached to its shadow cast component." << std::endl;

}