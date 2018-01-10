#include "SystemCollision.h"

SystemCollision::SystemCollision(EntityManager& pManager) : ISystem("SystemCollision", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_ROTATION | IComponent::COMPONENT_SCALE | IComponent::COMPONENT_COLLISION)), entityManager(pManager)
{
}

void SystemCollision::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		std::shared_ptr<ComponentRotation> rotComp = std::dynamic_pointer_cast<ComponentRotation> (entity.FindComponent(4));
		std::shared_ptr<ComponentScale> scaleComp = std::dynamic_pointer_cast<ComponentScale> (entity.FindComponent(8));
		std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (entity.FindComponent(32768));

		switch (collisionComp->type)
		{
		case ComponentCollision::collisionPrimitiveType::Sphere:
			for (unsigned int i = 0; i < entityManager.entityList.size(); i++)
			{
				if ((entityManager.entityList[i].mask & MASK) == MASK)
				{
					if (entityManager.entityList[i] == entity.)
					{

					}
				}
			}
			break;
		default:
			break;
		}

		collisionComp->type;

	}
}

SystemCollision::~SystemCollision()
{
}
