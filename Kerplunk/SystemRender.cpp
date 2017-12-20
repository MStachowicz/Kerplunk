#include "SystemRender.h"

SystemRender::SystemRender() : ISystem("SystemRender", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_GEOMETRY))
{}

void SystemRender::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::cout << "System: " << name << " Performing action on entity: " << entity.name << std::endl;

		std::shared_ptr<ComponentPosition> posComp =
			std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));

		std::shared_ptr<ComponentGeometry> velComp =
			std::dynamic_pointer_cast<ComponentGeometry> (entity.FindComponent(64));
	}
}

SystemRender::~SystemRender() {}
