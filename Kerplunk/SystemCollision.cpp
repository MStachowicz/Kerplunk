#include "SystemCollision.h"

SystemCollision::SystemCollision(EntityManager& pManager) : ISystem("SystemCollision", (IComponent::ComponentFlags)(IComponent::COMPONENT_RIGID_BODY | IComponent::COMPONENT_COLLISION)), entityManager(pManager)
{
}
SystemCollision::~SystemCollision()
{
}

void SystemCollision::OnLoad(const std::shared_ptr<Entity> &entity)
{
	if ((entity->mask & MASK) == MASK)
	{
	}
}
void SystemCollision::OnTickStart()
{
}
void SystemCollision::OnTickStart(const std::shared_ptr<Entity>& entity)
{
}
void SystemCollision::Tick(const std::shared_ptr<Entity>& entity)
{
	if ((entity->mask & MASK) == MASK)
	{
		std::shared_ptr<ComponentRigidBody> RigidBodyComponent = std::dynamic_pointer_cast<ComponentRigidBody> (entity->FindComponent(65536));
		std::shared_ptr<ComponentCollision> collisionComp = std::dynamic_pointer_cast<ComponentCollision> (entity->FindComponent(32768));


		for (unsigned int i = 0; i < entityManager.entityList.size(); i++)
		{
			if ((entityManager.entityList[i]->mask & MASK) == MASK) // if the entity is collidable 
			{
				if (entity != entityManager.entityList[i]) // if it is not the same entity as currently being checked
				{
					// Retrieve all the components that will be checked against.
					std::shared_ptr<ComponentRigidBody> RigidBodyComponent2 = std::dynamic_pointer_cast<ComponentRigidBody> (entityManager.entityList[i]->FindComponent(65536));
					std::shared_ptr<ComponentCollision> collisionComp2 = std::dynamic_pointer_cast<ComponentCollision> (entityManager.entityList[i]->FindComponent(32768));


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
							CollisionSphereSphere(entity, entityManager.entityList[i]);
							break;
						}
						case ComponentCollision::collisionPrimitiveType::Plane:
						{
							// SPHERE V PLANE CHECK
							// --------------------------------------------------------------------------------------------------
							CollisionSpherePlane(entity, entityManager.entityList[i]);
							break;
						}
						}
						break;
					}
				}
			}
		}
	}
}


bool SystemCollision::CollisionSphereSphere(const std::shared_ptr<Entity>& pSphereEntity, const std::shared_ptr<Entity>& pSphere2Entity)
{
	std::shared_ptr<ComponentRigidBody> sphereRigidBody = std::dynamic_pointer_cast<ComponentRigidBody> (pSphereEntity->FindComponent(65536));
	std::shared_ptr<ComponentRigidBody> sphere2RigidBody = std::dynamic_pointer_cast<ComponentRigidBody> (pSphere2Entity->FindComponent(65536));

	glm::vec3 sphere1toSphere2 = sphere2RigidBody->position - sphereRigidBody->position;
	float distance = glm::length(sphere1toSphere2) - (sphereRigidBody->scale + sphere2RigidBody->scale);

	if (distance < 0)
	{
		glm::vec3 relativeVelocity = sphereRigidBody->velocity - sphere2RigidBody->velocity;
		glm::vec3 collisionNormal = sphereRigidBody->position -	(sphereRigidBody->position + glm::normalize(sphere1toSphere2) * sphereRigidBody->scale);

		float restitution = 1.0f;

		if (glm::dot(relativeVelocity, collisionNormal) < 0)
		{
			float impulseMagnitude = glm::dot((-1 * (1 + restitution) * relativeVelocity), collisionNormal) /
				glm::dot(collisionNormal, (collisionNormal * ((1 / sphereRigidBody->mass) + (1 / sphere2RigidBody->mass))));

			// Apply an instant change in momentum/velocity using the calculated impulse
			sphereRigidBody->velocity = sphereRigidBody->velocity + ((impulseMagnitude / sphereRigidBody->mass) * collisionNormal);
			sphere2RigidBody->velocity = sphere2RigidBody->velocity - ((impulseMagnitude / sphere2RigidBody->mass) * collisionNormal);

			return true;
		}
	}

	return false;
}
// Finds the magnitude of the vector between the sphere and the closest point on a plane comparing it to the sphere radius.
bool SystemCollision::CollisionSpherePlane(const std::shared_ptr<Entity> &pSphereEntity, const std::shared_ptr<Entity> &pPlaneEntity)
{
	std::shared_ptr<ComponentRigidBody> sphereRigidBody = std::dynamic_pointer_cast<ComponentRigidBody> (pSphereEntity->FindComponent(65536));
	std::shared_ptr<ComponentRigidBody> planeRigidBody = std::dynamic_pointer_cast<ComponentRigidBody> (pPlaneEntity->FindComponent(65536));

	// Find the normal of the plane after its rotations
	glm::vec3 planeNormal = glm::vec3(0, 0, 1);

	// Rotate the normal by the plane rotations
	glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(planeRigidBody->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotMatrix = glm::rotate(rotMatrix, glm::radians(planeRigidBody->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotMatrix = glm::rotate(rotMatrix, glm::radians(planeRigidBody->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	planeNormal = glm::vec3(rotMatrix * glm::vec4(planeNormal, 1.0f));

	glm::vec3 sphereToClosestPoint = PlaneToPoint(planeNormal, planeRigidBody->position, sphereRigidBody->position) - sphereRigidBody->position;


	// The signed distance from the sphere center to the closest point on the plane
	float distance = glm::length(sphereToClosestPoint);
	float overlap = sphereRigidBody->scale - distance;


	// If a collision is detected
	if (overlap >= 0)
	{
		// Reverse the plane normal if sphere is behind the plane
		glm::vec3 sphereToPlane(sphereRigidBody->position - planeRigidBody->position);
		float dot = glm::dot(sphereToPlane, planeNormal);
		if (dot < 0)
		{
			planeNormal *= -1;
		}


		// Find how long ago (t) the collision occurred and move the sphere back that timestep
		float t = overlap / glm::length(sphereRigidBody->velocity);
		sphereRigidBody->position = sphereRigidBody->position + (t * -sphereRigidBody->velocity);

		// how far along the normal of the plane from sphere = collision point
		glm::vec3 collisionPoint = sphereRigidBody->position - (planeNormal * sphereRigidBody->scale);


		// Collision response
		glm::vec3 relativeVelocity = sphereRigidBody->velocity - planeRigidBody->velocity;
		glm::vec3 collisionNormal = glm::normalize(sphereRigidBody->position - collisionPoint);

		float restitution = 1.0f;

		float impulseMagnitude = glm::dot((-1 * (1 + restitution) * relativeVelocity), collisionNormal) /
			glm::dot(collisionNormal, (collisionNormal * ((1 / sphereRigidBody->mass) + (1 / planeRigidBody->mass))));

		if (glm::dot(relativeVelocity, collisionNormal) < 0)
		{
			sphereRigidBody->velocity = sphereRigidBody->velocity + ((impulseMagnitude / sphereRigidBody->mass) * collisionNormal);
			planeRigidBody->velocity = planeRigidBody->velocity - ((impulseMagnitude / planeRigidBody->mass) * collisionNormal);

			//log
		/*	std::cout << std::fixed <<
			"Collision with: " << pPlaneEntity.name <<
			"\nPlane normal: "  << planeNormal.x << ", " << planeNormal.y << ", " << planeNormal.z <<
			"\nCollision normal: " << collisionNormal.x << ", " << collisionNormal.y << ", " << collisionNormal.z <<
			"\nSphere position: " << sphereRigidBody->position.x << ", " << sphereRigidBody->position.y << ", " << sphereRigidBody->position.z <<
			"\nSphere velocity: " << sphereRigidBody->velocity.x << ", " << sphereRigidBody->velocity.y << ", " << sphereRigidBody->velocity.z << "\n\n" <<
			std::endl;*/
		}

		return true;
	}
	else
		return false;
}
// Returns the shortest vector from the plane to the point. (pg 165 collision detection)
glm::vec3 SystemCollision::PlaneToPoint(const glm::vec3 &pPlaneNormal, const glm::vec3 &pPlanePosition, glm::vec3& pPoint)
{
	float pointToPlaneDistance = glm::dot((pPoint - pPlanePosition), pPlaneNormal);

	// calculates how many units of plane normal length equal the point
	return glm::vec3(pPoint - (pointToPlaneDistance * pPlaneNormal));
}