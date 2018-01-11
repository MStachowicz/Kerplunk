#include "SystemCollision.h"

SystemCollision::SystemCollision(EntityManager& pManager) : ISystem("SystemCollision", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_ROTATION | IComponent::COMPONENT_SCALE | IComponent::COMPONENT_VELOCITY | IComponent::COMPONENT_COLLISION)), entityManager(pManager)
{
}

void SystemCollision::OnAction(Entity &entity)
{
	if ((entity.mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentPosition> posComp = std::dynamic_pointer_cast<ComponentPosition> (entity.FindComponent(2));
		std::shared_ptr<ComponentVelocity> velComp = std::dynamic_pointer_cast<ComponentVelocity> (entity.FindComponent(16));
		std::shared_ptr<ComponentScale> scaleComp = std::dynamic_pointer_cast<ComponentScale> (entity.FindComponent(8));
		std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (entity.FindComponent(32768));


		for (unsigned int i = 0; i < entityManager.entityList.size(); i++)
		{
			if ((entityManager.entityList[i].mask & MASK) == MASK) // if the entity is collidable 
			{
				if (entity != entityManager.entityList[i]) // if it is not the same entity as currently being checked
				{
					// Retrieve all the components that will be checked against.
					std::shared_ptr<ComponentPosition> posComp2 = std::dynamic_pointer_cast<ComponentPosition> (entityManager.entityList[i].FindComponent(2));
					std::shared_ptr<ComponentVelocity> velComp2 = std::dynamic_pointer_cast<ComponentVelocity> (entityManager.entityList[i].FindComponent(16));
					std::shared_ptr<ComponentScale> scaleComp2 = std::dynamic_pointer_cast<ComponentScale> (entityManager.entityList[i].FindComponent(8));
					std::shared_ptr<ComponentCollision> collisionComp2 = std::dynamic_pointer_cast<ComponentCollision> (entityManager.entityList[i].FindComponent(32768));


					switch (collisionComp->type)
					{
					case ComponentCollision::collisionPrimitiveType::Sphere:
						switch (collisionComp2->type)
						{
						case ComponentCollision::collisionPrimitiveType::Sphere:
							// SPHERE V SPHERE CHECK
							// --------------------------------------------------------------------------------------------------
							// The vector from the sphere to the sphere being checked against.
							glm::vec3 sphere1toSphere2 = posComp2->position - posComp->position;

							float radius1 = 1 * scaleComp->scale.x;
							float radius2 = 1 * scaleComp2->scale.x;
							float distance = glm::length(sphere1toSphere2) - (radius1 + radius2);

							if (distance <= 0)
							{
								velComp->Velocity *= -1;
								velComp2->Velocity *= -1;

								posComp->position += (glm::normalize(-sphere1toSphere2) * (abs(distance)));
								posComp2->position += (glm::normalize(sphere1toSphere2) * (abs(distance)));
							}
							break;
						}
						break;
					}
				}
			}

		}

		collisionComp->type;

	}
}

SystemCollision::~SystemCollision()
{
}
