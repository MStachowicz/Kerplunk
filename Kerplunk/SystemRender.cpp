#include "SystemRender.h"

SystemRender::SystemRender() : ISystem("SystemRender", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_ROTATION | IComponent::COMPONENT_SCALE |
	IComponent::COMPONENT_GEOMETRY | IComponent::COMPONENT_SHADER))
{}

void SystemRender::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));

		std::shared_ptr<ComponentGeometry> geomComp =
			std::dynamic_pointer_cast<ComponentGeometry> (entity.FindComponent(64));

		std::shared_ptr<ComponentShader> shaderComp =
			std::dynamic_pointer_cast<ComponentShader> (entity.FindComponent(128));
	}
}

void SystemRender::setShaderModel(std::shared_ptr<Shader> shader, glm::vec3 & position, glm::vec3 & rotation, glm::vec3 & scale)
{
	shader->use();
}

SystemRender::~SystemRender() {}
