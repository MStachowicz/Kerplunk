#ifndef SYSTEMSHADOWS_H
#define SYSTEMSHADOWS_H

#include "ISystem.h"
#include "EntityManager.h"
#include "ComponentShadowCast.h"
#include "ComponentDirectionalLight.h"
#include "ComponentSpotlight.h"
#include "ComponentPointLight.h"
#include "ComponentRenderable.h"
#include "ComponentRigidBody.h"
#include "ComponentGeometry.h"
#include "componentModel.h"

class SystemShadows : public ISystem
{
public:
	SystemShadows(EntityManager& pManager);
	~SystemShadows();

	// Vector of all the entities that can be rendered.
	std::vector<std::weak_ptr<Entity>> renderableEntities;
	// Shader passed to all shadow casters to use for filling their depth buffers.
	std::shared_ptr<Shader> directionalDepthShader;

	void OnLoad(const std::shared_ptr<Entity> &entity);
	void OnTickStart(const std::shared_ptr<Entity> &entity);
	void OnTickStart();
	void Tick(const std::shared_ptr<Entity> &entity);

	void FillDepthBuffer(const std::shared_ptr<Entity> &entity);
};
#endif