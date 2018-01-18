#include "SystemCollision.h"

SystemCollision::SystemCollision(EntityManager& pManager) : ISystem("SystemCollision", (IComponent::ComponentFlags)(IComponent::COMPONENT_POSITION | IComponent::COMPONENT_ROTATION | IComponent::COMPONENT_SCALE | IComponent::COMPONENT_VELOCITY | IComponent::COMPONENT_COLLISION)), entityManager(pManager)
{
}

void SystemCollision::OnLoad(Entity &entity) 
{
	if ((entity.mask & MASK) == MASK)
	{
	}
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
						{
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
						case ComponentCollision::collisionPrimitiveType::Plane:
						{						
							CollisionSpherePlane(entity, entityManager.entityList[i]);				
							break;
						}
						}
						break;
					}
				}
			}

		}

		collisionComp->type;

	}
}

// Finds the magnitude of the vector between the sphere and the closest point on a plane comparing it to the sphere radius.
bool SystemCollision::CollisionSpherePlane(Entity& pSphereEntity, Entity& pPlaneEntity)
{
	std::shared_ptr<ComponentPosition> spherePos = std::dynamic_pointer_cast<ComponentPosition> (pSphereEntity.FindComponent(2));
	std::shared_ptr<ComponentScale> sphereScale = std::dynamic_pointer_cast<ComponentScale> (pSphereEntity.FindComponent(8));
	std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (pSphereEntity.FindComponent(32768));

	glm::vec3 sphereToClosestPoint = ClosestPtPlane(pPlaneEntity, spherePos->position) - spherePos->position;
	// The signed distance from the sphere center to the closest point on the plane
	float distance = glm::length(sphereToClosestPoint);

	// If a collision is detected log it to the collision component of the sphere.
	if (abs(distance) <= 1 * sphereScale->scale.x)
	{
		collisionComp->AddCollision(pPlaneEntity, sphereToClosestPoint + spherePos->position);
		return true;
	}
	else
		return false;
}

// Returns the closest point on a plane to a point.
glm::vec3 SystemCollision::ClosestPtPlane(Entity& pPlane, glm::vec3& pPoint)
{
	std::shared_ptr<ComponentPosition> planePos = std::dynamic_pointer_cast<ComponentPosition> (pPlane.FindComponent(2));	

	// pg 165 collision detection
	glm::vec3 planeNormal = glm::vec3(0, 0, 1);
	// calculates how many units of plane normal length equal the point
	float t = glm::dot(planeNormal, (pPoint - planePos->position));

	return glm::vec3(pPoint - (t * planeNormal));
}


SystemCollision::~SystemCollision()
{
}
