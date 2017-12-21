#include "SystemRender.h"

SystemRender::SystemRender() : ISystem("SystemRender", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_ROTATION | IComponent::COMPONENT_SCALE |
	IComponent::COMPONENT_GEOMETRY | IComponent::COMPONENT_SHADER | IComponent::COMPONENT_TEXTURE))
{}

void SystemRender::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));

		std::shared_ptr<ComponentRotation> rotComp =
			std::dynamic_pointer_cast<ComponentRotation> (entity.FindComponent(4));

		std::shared_ptr<ComponentScale> scaleComp =
			std::dynamic_pointer_cast<ComponentScale> (entity.FindComponent(8));

		std::shared_ptr<ComponentGeometry> geomComp =
			std::dynamic_pointer_cast<ComponentGeometry> (entity.FindComponent(64));

		std::shared_ptr<ComponentShader> shaderComp =
			std::dynamic_pointer_cast<ComponentShader> (entity.FindComponent(128));

		std::shared_ptr<ComponentTexture> texComp =
			std::dynamic_pointer_cast<ComponentTexture> (entity.FindComponent(256));
	}
}

void SystemRender::setShaderModel(std::shared_ptr<Shader> shader, glm::vec3 & position, glm::vec3 & rotation, glm::vec3 & scale)
{
	shader->use();
}

SystemRender::~SystemRender() {}
